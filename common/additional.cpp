/// \file
/// \brief Заголовочный файл, содержащий функции для работы со строками и текущим каталогом.

#include "additional.h"

/// \namespace Prog1
/// \brief Пространство имен для основных функций работы со строками и каталогами.

namespace Prog1 {

    /** \brief Эта функция считывает строку из стандартного ввода (std::cin).
     \return Указатель на динамически выделенную строку в случае успешного ввода.
             Если ввод не удался (например, достигнут EOF или произошла ошибка),
             возвращается nullptr. Если не удалось корректно выделить память, обработчик исключений
             увидит ошибку и с помощью блока try catch корректно ее обработает, после чего программа
             продолжит работу

     \note Пользователь должен освободить выделенную память после использования.
          Например, использовать delete[] для освобождения.
    */
    char* input_string() {
        std::string result;
        getline(std::cin, result);
        if (std::cin.eof())
            return nullptr;
        if (std::cin.fail()) {
            std::cin.clear();
            return nullptr;
        }
        try {
            char* buffer = new char[result.size() + 1];
            std::copy(result.begin(), result.end(), buffer);
            buffer[result.size()] = '\0';
            return buffer;
        } catch (const std::bad_alloc& e) {
            std::cerr << "Failed to allocate memory: " << e.what() << std::endl;
            throw;
        }
    }

    /** \brief Получает текущий рабочий каталог.

     Эта функция получает текущий рабочий каталог с использованием системного вызова getcwd.
     \return Строка, содержащая путь к текущему рабочему каталогу. Возвращает пустую строку
             в случае ошибки, например, если размер буфера недостаточен и ошибка не связана
             с ERANGE.

     \exception Если возникла ошибка, кроме ERANGE, будет возвращена пустая строка.
    */

    std::string get_current_dir() {
        std::vector<char> buffer(BUFSIZ);
        while (true) {
            if (getcwd(buffer.data(), buffer.size()) != nullptr) {
                return { std::string(buffer.data()) };
            }
            if (errno != ERANGE) {
                return "";
            }
            buffer.resize(buffer.size() + BUFSIZ);
        }
    }

    /**
     @brief Обрабатывает ошибки ввода из потока

     @param in Поток ввода
     @throw std::runtime_error Если произошла ошибка ввода

     Функция проверяет поток ввода на наличие ошибок и выбрасывает исключение, если произошла ошибка.
     Если был достигнут конец файла, то выбрасывается исключение с сообщением "EOF".
     Если произошла ошибка ввода, то поток ввода очищается и игнорируются все символы до символа новой строки.
    */
    extern void process_error(std::istream& in) {
        if (in.bad())
            throw std::runtime_error("Bad stream");
        if (in.eof())
            throw std::runtime_error("EOF");
        if (in.fail()) {
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    /**
      @brief Читает целое число из входного потока.

      @param in Входной поток, из которого нужно прочитать число.
      @return Прочитанное целое число.

      Функция читает целое число из входного потока. Если чтение не удалось,
      то функция вызывает функцию process_error() и продолжает попытки чтения,
      пока не получит корректное значение.
     */
    extern int readint(std::istream& in) {
        int res;
        in >> res;
        while(!in.good()) {
            process_error(in);
            in >> res;
        }
        return res;
    }

    /**
    @brief Читает строку из входного потока.

    @param in Входной поток, из которого нужно прочитать строку.
    @return Прочитанная строка.

    Функция читает строку из входного потока. Если чтение не удалось,
    то функция вызывает функцию process_error() и продолжает попытки чтения,
    пока не получит корректное значение.
     */
    extern std::string readline(std::istream& in) {
        std::string res;
        std::getline(in, res);
        while(!in.good()) {
            process_error(in);
            std::getline(in, res);
        }
        return res;
    }
}
