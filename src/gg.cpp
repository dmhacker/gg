#include <iostream>

#include <readline/readline.h>
#include <readline/history.h>

#include "gg.hpp"

void display_write(GDB & gdb, std::string & gdb_output, std::string & gdb_error) {
  // Clear string buffers 
  gdb_output.clear();
  gdb_error.clear();

  // Read from GDB to populate buffers
  gdb.read_until_prompt(gdb_output, gdb_error, true);

  if (!gdb_error.empty() || !gdb_output.empty()) {
    // Pass output to IO streams
    std::cerr << gdb_error << std::flush;
    std::cout << gdb_output << std::flush;
  }
}

int main(int argc, char ** argv) {
  // Convert char ** to std::vector<std::string>
  std::vector<std::string> argv_vector;
  for (int i = 0; i < argc; i++) {
    char * arg = argv[i];
    std::string arg_string(arg);
    argv_vector.push_back(arg_string);
  }

  // Create GDB process
  GDB gdb(argv_vector);

  // Create string buffers 
  std::string gdb_output;
  std::string gdb_error;

  // Display gdb introduction to user 
  display_write(gdb, gdb_output, gdb_error);

  // TODO: currently blocking, offload to separate thread
  wxEntry(argc, argv);

  while (gdb.is_running()) {
    // Read one line from stdin to process (blocking)
    char * buf_input = readline(GDB_PROMPT);

    // A null pointer signals input EOF 
    if (!buf_input) {
      std::cout << GDB_QUIT << std::endl;
      break;
    }

    // Handle empty input separately from regular command
    if (strlen(buf_input)) {
      // Add input to our CLI history
      add_history(buf_input);

      // Execute the command that we read in 
      std::string gdb_input(buf_input);
      gdb.execute(gdb_input);

      // Display the result of the command and the next prompt
      display_write(gdb, gdb_output, gdb_error);
    }
    else {
      // Display the prompt for the next command 
      std::cout << GDB_PROMPT;
    }

    // Delete the input buffer, free up memory
    delete buf_input;
  }
}
