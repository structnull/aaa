use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::process::exit;

const MAX_VAR_NAME: usize = 20;

struct Interpreter {
    variables: HashMap<String, i32>,
}

impl Interpreter {
    fn new() -> Self {
        Self {
            variables: HashMap::new(),
        }
    }

    fn is_valid_variable_name(name: &str) -> bool {
        let mut chars = name.chars();
        chars.next().map_or(false, |c| c.is_alphabetic())
            && chars.all(|c| c.is_alphanumeric() || c == '_')
    }

    fn get_variable_value(&self, name: &str) -> Result<i32, String> {
        self.variables
            .get(name)
            .cloned()
            .ok_or_else(|| format!("Error: Variable '{}' not found.", name))
    }

    fn set_variable(&mut self, name: &str, value: i32) -> Result<(), String> {
        if !Self::is_valid_variable_name(name) {
            return Err(format!("Error: Invalid variable name '{}'.", name));
        }

        if self.variables.len() < MAX_VAR_NAME {
            self.variables.insert(name.to_string(), value);
            Ok(())
        } else {
            Err("Error: Maximum number of variables reached.".to_string())
        }
    }

    fn execute_print(&self, arg: &str) {
        if arg.starts_with('"') && arg.ends_with('"') {
            println!("{}", &arg[1..arg.len() - 1]);
        } else if let Ok(num) = arg.parse::<i32>() {
            println!("{}", num);
        } else {
            match self.get_variable_value(arg) {
                Ok(value) => println!("{}", value),
                Err(e) => eprintln!("{}", e),
            }
        }
    }

    fn execute_declaration(&mut self, var_name: &str) {
        let _ = self.set_variable(var_name, 0);
    }

    fn execute_assignment(&mut self, var_name: &str, value: i32) {
        let _ = self.set_variable(var_name, value);
    }

    fn execute_increment(&mut self, var_name: &str) {
        if let Ok(value) = self.get_variable_value(var_name) {
            let _ = self.set_variable(var_name, value + 1);
        }
    }

    fn execute_if(&mut self, var_name: &str, expected_value: i32, block: &str) {
        if let Ok(value) = self.get_variable_value(var_name) {
            if value == expected_value {
                self.interpreter(block);
            }
        }
    }

    fn interpreter(&mut self, line: &str) {
        let line = line.split('#').next().unwrap_or("").trim(); // Remove comments
        if line.is_empty() {
            return;
        }

        for statement in line.split('.') {
            let statement = statement.trim();
            if statement.is_empty() {
                continue;
            }

            match statement {
                s if s.starts_with("anjima") => {
                    let arg = s.trim_start_matches("anjima ").trim();
                    self.execute_print(arg);
                }
                s if s.starts_with("adharsh") => {
                    let var_name = s.trim_start_matches("adharsh ").trim();
                    self.execute_declaration(var_name);
                }
                "anakha" => exit(0),
                s if s.contains(" = ") => {
                    let parts: Vec<&str> = s.split(" = ").collect();
                    if parts.len() == 2 {
                        let var_name = parts[0].trim();
                        if let Ok(value) = parts[1].trim().parse::<i32>() {
                            self.execute_assignment(var_name, value);
                        } else {
                            eprintln!("Syntax error: Invalid assignment value: {}", parts[1].trim());
                        }
                    } else {
                        eprintln!("Syntax error: Invalid assignment: {}", s);
                    }
                }
                s if s.ends_with("++") => {
                    let var_name = s.trim_end_matches("++").trim();
                    self.execute_increment(var_name);
                }
                s if s.starts_with("if (") && s.contains(") {") && s.ends_with('}') => {
                    if let Some(condition_start) = s.find('(') {
                        if let Some(condition_end) = s.find(") {") {
                            let condition = &s[condition_start + 1..condition_end].trim();
                            if let Some(block_start) = s.find('{') {
                                let block = &s[block_start + 1..s.len() - 1].trim();

                                let condition_parts: Vec<&str> = condition.split("==").collect();
                                if condition_parts.len() == 2 {
                                    let var_name = condition_parts[0].trim();
                                    if let Ok(expected_value) = condition_parts[1].trim().parse::<i32>() {
                                        self.execute_if(var_name, expected_value, block);
                                    } else {
                                        eprintln!("Syntax error: Invalid expected value in if condition.");
                                    }
                                } else {
                                    eprintln!("Syntax error: Invalid if condition format.");
                                }
                            }
                        }
                    }
                }
                _ => {
                    eprintln!("Syntax error: {}", statement);
                }
            }
        }
    }
}

fn main() -> io::Result<()> {
    let args: Vec<String> = std::env::args().collect();
    if args.len() != 2 {
        eprintln!("Usage: {} <filename>", args[0]);
        return Ok(());
    }

    let file = File::open(&args[1])?;
    let reader = BufReader::new(file);

    let mut interpreter = Interpreter::new();

    for line in reader.lines() {
        if let Ok(line) = line {
            interpreter.interpreter(&line);
        }
    }

    Ok(())
}
