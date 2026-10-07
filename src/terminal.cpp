#include "includes/terminal.hpp"

#include <termios.h>
#include <cerrno>
#include <unistd.h>
#include <poll.h>

static struct termios original_termios;

void terminal_raw_mode()
{
    tcgetattr(STDIN_FILENO, &original_termios);

    struct termios raw = original_termios;

    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void terminal_restore()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

KeyEvent terminal_read_key()
{
    char c;


    int result = read(STDIN_FILENO, &c, 1);

    if (result < 0 && errno == EINTR)
        return {Key::CTRL_C, 0};

    if (result != 1)
        return {Key::NONE, 0};

    if (c == 3)
        return {Key::CTRL_C, 0};

    if (c == '\n' || c == '\r')
        return {Key::ENTER, 0};

    if (c == 127 || c == 8)
        return {Key::BACKSPACE, 0};

    if (c == 27)
    {
        struct pollfd pfd;
        pfd.fd = STDIN_FILENO;
        pfd.events = POLLIN;

        int ready = poll(&pfd, 1, 50);

        if (ready <= 0)
            return {Key::ESCAPE, 0};

        char next;

        if (read(STDIN_FILENO, &next, 1) != 1)
            return {Key::ESCAPE, 0};

        if (next != '[')
            return {Key::ESCAPE, 0};

        if (read(STDIN_FILENO, &next, 1) != 1)
            return {Key::ESCAPE, 0};

        if (next == 'A')
            return {Key::UP, 0};
        if (next == 'B')
            return {Key::DOWN, 0};
        if (next == 'C')
            return {Key::RIGHT, 0};
        if (next == 'D')
            return {Key::LEFT, 0};

        return {Key::NONE, 0};
    }

    return {Key::CHAR, c};
}
