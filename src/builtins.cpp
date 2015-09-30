#include "builtins.h"

using namespace std;

int ct_val = 0;
std::map<int,string> commands;
std::map<string,string> aliases;
std::map<int,string>::iterator it = commands.begin();
std::map<string,string>::iterator it_a = aliases.begin();


int com_ls(vector<string>& tokens) {
  string com = "ls ";
  // if no directory is given, use the local directory
  if (tokens.size() < 2) {
    tokens.push_back(".");
  } else {
    com += tokens[1].c_str();
  }
  
  // open the directory
  DIR* dir = opendir(tokens[1].c_str());

  // catch an errors opening the directory
  if (!dir) {
    // print the error from the last system call with the given prefix
    perror("ls error: ");

    // return error
    return 1;
  } else {
    addCommand(com);
  }

  // output each entry in the directory
  for (dirent* current = readdir(dir); current; current = readdir(dir)) {
    cout << current->d_name << endl;
  }

  // return success
  return 0;
}


int com_cd(vector<string>& tokens) {
  // TODO: YOUR CODE GOES HERE
  string com = "cd ";
  if(tokens.size() >= 2)
    com += tokens[1].c_str();
  // directory is second token
  string new_dir = tokens[1].c_str(); 
  int rc = chdir(new_dir.c_str());
  // display error
  if(rc > 0) {
    perror("cd error: ");
    return rc;
  } else {
    addCommand(com);
  }  
  return 0;
}


int com_pwd(vector<string>& tokens) {
  // TODO: YOUR CODE GOES HERE
  addCommand("pwd");
  // HINT: you should implement the actual fetching of the current directory in
  // pwd(), since this information is also used for your prompt
  cout << pwd() << endl;
  return 0;
}


int com_alias(vector<string>& tokens) {
  // TODO: YOUR CODE GOES HERE
  string command = "alias "; 
  string line = tokens[0].c_str();
  // display all stored aliases
  if(tokens.size() < 2) {
    displayAliases();
  } else { // add alias to map
    string::size_type eq_pos = tokens[1].find("=");

    // If there is an equal sign in the token, assume the token is var=value
    if (eq_pos != string::npos) {
        string name = tokens[1].substr(0, eq_pos);
        string value = tokens[1].substr(eq_pos + 1);
        addAlias(name,value);
    }
    command += tokens[1].c_str(); 
  }
  addCommand(command);
  
  return 0;
}


int com_unalias(vector<string>& tokens) {
  // TODO: YOUR CODE GOES HERE
  string command = "unalias ";
  if(tokens.size() >= 2) {
    command += tokens[1].c_str();
    if(strcmp(tokens[1].c_str(),"-a") == 0) {
        aliases.clear();
    } else {
        removeAlias(tokens[1].c_str());
    }
  }
  addCommand(command);

  return 0;
}


int com_echo(vector<string>& tokens) {
  // TODO: YOUR CODE GOES HERE
  string comment = "echo ";
  for(int i = 1; i < tokens.size(); i++) {
    cout << tokens[i] << " ";
    comment += tokens[i] + " ";
  }
  addCommand(comment);
  return 0;
}


int com_exit(vector<string>& tokens) {
  // TODO: YOUR CODE GOES HERE
  addCommand("exit");
  exit(0);
  return 0;
}


int com_history(vector<string>& tokens) {
  // TODO: YOUR CODE GOES HERE
  string history = "history";
  if(tokens.size() >= 2) {
    if(tokens[1] == "!!") {
        history += " !!";
        int last = commands.size() - 1;
        string last_command = commands.at(last);
        //cout << "last: " << last_command << endl;
        vector<string> command_tokens;
        string token;

        // istringstream allows us to treat the string like a stream
        istringstream token_stream(last_command);
        command_tokens.clear();

        while (token_stream >> token) {
            command_tokens.push_back(token);
        }
        builtInCommand(command_tokens);
        
    } else {
        string n = tokens[1].substr(1,2);
        history += " !" + n;
        string n_command = commands.at(std::stoi(n));
        
        //cout << "nth: " << n_command << endl;
        vector<string> command_tokens;
        string token;

        // istringstream allows us to treat the string like a stream
        istringstream token_stream(n_command);
        command_tokens.clear();

        while (token_stream >> token) {
            command_tokens.push_back(token);
        }
        builtInCommand(command_tokens);
    }
  } else { 
    displayHistory();
  }
  addCommand(history);
  
  return 0;
}

string pwd() {
  string pwd_str = getcwd(NULL,0);
  if(pwd_str.length() > 0)
    return pwd_str;
  return NULL;
}

void addCommand(string command) {
    commands.insert(it, std::pair<int,string>(ct_val,command));
    ct_val++;
}

void displayHistory() {
    for(it=commands.begin(); it!=commands.end(); ++it) {
        cout << it->first << "  " << it->second << endl;
    }
}

void addAlias(string key, string value) {
    aliases.insert(it_a, std::pair<string,string>(key,value));
}

void removeAlias(string key) {
    it_a=aliases.find(key);
    aliases.erase(it_a);
}

void displayAliases() {
    for(it_a=aliases.begin(); it_a!=aliases.end(); ++it_a) {
        cout << it_a->first << "  " << it_a->second << endl;
    }
}

void builtInCommand(vector<string>& tokens) {
    int n = tokens.size();
    string token = tokens[0].c_str();
    if(strcmp(token.c_str(),"ls") == 0) {
        //cout << "ls" << endl;
        com_ls(tokens);
    } else if(strcmp(token.c_str(),"cd") == 0) {
        //cout << "cd" << endl;
        com_cd(tokens);
    } else if(strcmp(token.c_str(),"pwd") == 0) {
        //cout << "pwd" << endl;
        com_pwd(tokens);
    } else if(strcmp(token.c_str(),"exit") == 0) {
        //cout << "exit" << endl;
        com_exit(tokens);
    } else if(strcmp(token.c_str(),"echo") == 0) {
        //cout << "echo" << endl;
        com_echo(tokens);
    } else if(strcmp(token.c_str(),"history") == 0) {
        //cout << "history" << endl;
        com_history(tokens);
    } else if(strcmp(token.c_str(),"alias") == 0) {
        //cout << "alias" << endl;
        com_alias(tokens);
    } else if(strcmp(token.c_str(),"unalias") == 0) {
        //cout << "unalias" << endl;
        com_unalias(tokens);
    }
}
