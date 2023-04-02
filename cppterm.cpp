#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  std::string input;
  std::vector<std::string> args;

  while (true) {
    std::cout << "root@cppterm> ";
    std::getline(std::cin, input);

    args.clear();
    const std::string delimiter = " ";
    size_t pos = 0;

    while ((pos = input.find(delimiter)) != std::string::npos) {
      args.emplace_back(input.substr(0, pos));
      input.erase(0, pos + delimiter.length());
    }

    if (!input.empty()) {
      args.emplace_back(input);
    }

    std::vector<char*> argv(args.size() + 1);
    for (size_t i = 0; i < args.size(); i++) {
      argv[i] = args[i].data();
    }
    argv[args.size()] = nullptr;

    pid_t pid = fork();

    if (pid == -1) {
      std::cerr << "Erreur lors de la création du processus\n";
      continue;
    } else if (pid == 0) {
      execvp(argv[0], argv.data());
      std::cerr << "Erreur d'exécution de la commande\n";
      std::exit(EXIT_FAILURE);
    } else {
      int status;
      waitpid(pid, &status, 0);
    }
  }

  return 0;
}
