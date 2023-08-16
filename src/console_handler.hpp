
#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#elif __linux__
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#endif
extern int console_width;
int get_console_width();
bool is_input_available();
char get_char_no_echo();
void clear_console();
void emergency_refresh_screen();
#ifdef CONSOLE_HANDLER_IMPLEMENTATION

int console_width = 0;

void emergency_refresh_screen() {
    console_width = get_console_width();

}

void clearConsole() {
#ifdef _WIN32
    std::system("cls");
#elif __linux__
    std::system("clear");
#endif
}

int get_console_width()
{
  int console_width = 0;

#ifdef _WIN32
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  if (GetConsoleScreenBufferInfo(console, &csbi))
  {
    console_width = csbi.dwSize.X;
  }
#elif __linux__
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  console_width = w.ws_col;
#endif

  return console_width;
}
// Function to check if input is available in the console buffer
bool is_input_available()
{
#ifdef _WIN32
  return _kbhit() != 0;
#elif __linux__
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &timeout) > 0;
#endif
}

// Function to get a character from the console without echoing it
char get_char_no_echo()
{
  if (!is_input_available())
  {
    return '\0'; // Return null character if no input is available
  }

#ifdef _WIN32
  return _getch();
#elif __linux__
  struct termios oldt, newt;
  char ch;

  tcgetattr(STDIN_FILENO, &oldt); // Save current terminal settings
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore original terminal settings

  return ch;
#endif
}

#endif