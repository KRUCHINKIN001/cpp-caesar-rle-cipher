#include <iostream>
#include <string>
#include <windows.h>

/**
 * Проверяет строку:
 * длина от 1 до 100 символов, разрешены только цифры.
 */
bool checkText(const std::string& text)
{
    if (text.empty() || text.length() > 100) {
        return false;
    }

    for (char symbol : text) {
        if (symbol < '0' || symbol > '9') {
            return false;
        }
    }

    return true;
}

/**
 * Шифрует цифры с постоянным ключом 3.
 */
std::string encrypt(const std::string& text)
{
    std::string result = "";

    // Сдвинем каждую цифру на три позиции вперёд.
    for (char symbol : text) {
        int digit = symbol - '0';

        digit = (digit + 3) % 10;

        result += char('0' + digit);
    }

    return result;
}

/**
 * Сжимает строку алгоритмом RLE.
 * compressed хранит запись без разделителей.
 * separated хранит запись с разделителем после каждого счётчика.
 */
void compressRLE(const std::string& text,
                 std::string& compressed,
                 std::string& separated)
{
    compressed = "";
    separated = "";

    int length = text.length();
    int i = 0;

    while (i < length) {
        char symbol = text[i];
        int count = 1;

        // Посчитаем подряд идущие одинаковые цифры.
        while (i + count < length &&
               text[i + count] == symbol) {
            count++;
        }

        compressed += symbol;
        compressed += std::to_string(count);

        // Добавим разделитель для однозначного восстановления.
        separated += symbol;
        separated += std::to_string(count);
        separated += ';';

        i += count;
    }
}

/**
 * Восстанавливает зашифрованную строку из RLE с разделителями.
 * Получает только корректный результат функции compressRLE.
 */
std::string restoreRLE(const std::string& text)
{
    std::string result = "";

    int length = text.length();
    int i = 0;

    while (i < length) {
        char symbol = text[i];
        i++;

        int count = 0;

        // Прочитаем все цифры количества повторений до разделителя.
        while (i < length && text[i] != ';') {
            count = count * 10 + (text[i] - '0');
            i++;
        }

        // Пропустим разделитель после количества повторений.
        i++;

        for (int j = 0; j < count; j++) {
            result += symbol;
        }
    }

    return result;
}

/**
 * Расшифровывает цифры, сдвигая их на три позиции назад.
 */
std::string decrypt(const std::string& text)
{
    std::string result = "";

    for (char symbol : text) {
        int digit = symbol - '0';

        digit = (digit + 10 - 3) % 10;

        result += char('0' + digit);
    }

    return result;
}

/**
 * Получает строку, шифрует и сжимает её.
 * Затем выполняет обратные преобразования и проверяет результат.
 */
int main()
{
    // Настроим консоль под кодировку Windows-1251.
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    std::string original;
    std::string encrypted;
    std::string compressed;
    std::string separated;
    std::string restoredEncrypted;
    std::string restoredOriginal;

    std::cout << "Шифрование и сжатие числового текста\n";
    std::cout << "Ключ шифрования: 3\n";
    std::cout << "Введите строку из цифр (не более 100 символов): ";

    // Прочитаем строку полностью, чтобы обнаружить и пробелы.
    if (!std::getline(std::cin, original)) {
        std::cout << "Ошибка чтения строки.\n";
        return 1;
    }

    if (!checkText(original)) {
        std::cout << "Ошибка: введите от 1 до 100 цифр.\n";
        std::cout << "Буквы, пробелы и другие символы запрещены.\n";
        return 1;
    }

    // Сначала зашифруем текст, затем выполним сжатие.
    encrypted = encrypt(original);
    compressRLE(encrypted, compressed, separated);

    std::cout << "\nИсходная строка: " << original << "\n";
    std::cout << "Зашифрованная строка: " << encrypted << "\n";
    std::cout << "Сжатая строка: " << compressed << "\n";
    std::cout << "RLE с разделителями: " << separated << "\n";

    // Выполним обратные действия: восстановление RLE и расшифровку.
    restoredEncrypted = restoreRLE(separated);
    restoredOriginal = decrypt(restoredEncrypted);

    std::cout << "\nПосле восстановления RLE: "
              << restoredEncrypted << "\n";

    std::cout << "После расшифровки: "
              << restoredOriginal << "\n";

    if (restoredOriginal == original) {
        std::cout << "Проверка: строки совпадают.\n";
    } else {
        std::cout << "Проверка: строки НЕ совпадают.\n";
    }

    return 0;
}
