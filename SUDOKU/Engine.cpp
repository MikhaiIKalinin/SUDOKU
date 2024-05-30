#include "Engine.h"

// Конструктор класу Engine, який ініціалізує генератор випадкових чисел і завантажує шрифт
Engine::Engine()
{
    srand(time(NULL));

    font.loadFromFile("Raleway-Black.ttf");

    Game game;
    state = GAME;
}

// Деструктор класу Engine
Engine::~Engine()
{
}

// Функція runEngine відповідає за основний цикл гри
bool Engine::runEngine(RenderWindow& window, int level) {
    int size = 4;
    int index = 0;

    // Створення динамічних двовимірних масивів для зберігання значень та стану блокування клітинок
    int** m = new int* [size];
    for (int i = 0; i < size; i++) {
        m[i] = new int[size];
    }

    bool** b = new bool* [size];
    for (int i = 0; i < size; i++) {
        b[i] = new bool[size];
    }

    // Ініціалізація масиву b значеннями true
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            b[i][j] = true;
        }
    }

    // Заповнення таблиці та встановлення рівня складності
    fillTable(m, size, window);
    setLevel(m, b, size, level);

    Input input;
    input.val = 0;
    input.value = "";

    CellSize = 100;
    FontSize = 50;

    // Основний цикл гри
    while (window.isOpen()) {
        Event event;

        // Обробка подій
        while (window.pollEvent(event) && state == GAME) {
            switch (event.type) {
            case Event::Closed:
                // Звільнення пам'яті при закритті вікна
                for (int i = 0; i < size; i++) {
                    delete[] m[i];
                }
                delete[] m;

                for (int i = 0; i < size; i++) {
                    delete[] b[i];
                }
                delete[] b;

                window.close();
                break;

            case Event::KeyPressed:
                if ((Keyboard::isKeyPressed(Keyboard::Escape))) {
                    // Звільнення пам'яті при натисканні Escape
                    for (int i = 0; i < size; i++) {
                        delete[] m[i];
                    }
                    delete[] m;

                    for (int i = 0; i < size; i++) {
                        delete[] b[i];
                    }
                    delete[] b;

                    return false;
                }
                // Вибір клітинки
                selectCell(window, size, index, m, b, input);
                break;

            default:
                break;
            }
        }

        // Очищення вікна і малювання елементів гри
        window.clear();
        drawSquare(window, m, b, size, index);

        input.cell.setCharacterSize(FontSize);
        input.cell.setFillColor(Color(80, 80, 80));
        input.cell.setFont(font);
        input.cell.setString(input.value);
        input.cell.setPosition(10, 10);
        input.cell.setPosition((400 - size / 2 * CellSize) + index % size * CellSize + FontSize / 2, (300 - size / 2 * CellSize) + index / size * CellSize + FontSize / 2);

        window.draw(input.cell);

        // Перевірка на виграш
        if (checkWin(m, size)) {
            state = GAMEOVER;
        }

        if (state == GAMEOVER) {
            string text = "Finish";

            Text content(text, font, 60);
            content.setPosition(300, 25);
            content.setFillColor(Color::Red);
            window.draw(content);

            if ((Keyboard::isKeyPressed(Keyboard::Escape))) {
                // Звільнення пам'яті при натисканні Escape
                for (int i = 0; i < size; i++) {
                    delete[] m[i];
                }
                delete[] m;

                for (int i = 0; i < size; i++) {
                    delete[] b[i];
                }
                delete[] b;

                return false;
            }
        }
        window.display();
    }
    return true;
}

// Перевірка рядка на наявність значення
bool Engine::checkRow(int** m, int size, int row, int value)
{
    for (int i = 0; i < size; i++)
    {
        if (m[row][i] == value)
        {
            return false;
        }
    }
    return true;
}

// Перевірка колонки на наявність значення
bool Engine::checkColumn(int** m, int size, int column, int value)
{
    for (int i = 0; i < size; i++)
    {
        if (m[i][column] == value)
        {
            return false;
        }
    }
    return true;
}

// Перевірка квадрата 2x2 на наявність значення
bool Engine::checkSquare(int** m, int tamanho, int row, int column, int value)
{
    int quadx = 2, quady = 2;

    int sizey = row / quady;
    int sizex = column / quadx;

    sizey = (sizey * quady);
    sizex = (sizex * quadx);

    for (int i = sizey; i < sizey + quady; i++)
    {
        for (int j = sizex; j < sizex + quadx; j++)
        {
            if (m[i][j] == value)
            {
                return false;
            }
        }
    }
    return true;
}

// Перевірка, чи всі елементи масиву false
bool Engine::removing(bool bol[16], int t)
{
    for (int i = 0; i < t; i++)
    {
        if (bol[i])
        {
            return false;
        }
    }
    return true;
}

// Обробка вибору клітинки та введення значень
void Engine::selectCell(RenderWindow& window, int size, int& index, int** matrix, bool** bloc, Input& input)
{
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        index--;
        if (index < 0)
        {
            index = size * size - 1;
        }
        input.value = "";
    }

    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        index++;
        if (index > size * size - 1)
        {
            index = 0;
        }
        input.value = "";

    }
    if (Keyboard::isKeyPressed(Keyboard::Up))
    {
        index -= size;
        if (index < 0)
        {
            index = ((size * size)) - size + ((index + size) % size);
        }
        input.value = "";
    }

    if (Keyboard::isKeyPressed(Keyboard::Down))
    {
        index += size;
        if (index > size * size - 1)
        {
            index = index % size;
        }
        input.value = "";
    }
    if (!bloc[index / size][index % size] && input.value.size() <= 1)
    {
        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            input.value += 'A';
        }
        if (Keyboard::isKeyPressed(Keyboard::B))
        {
            input.value += 'B';
        }
        if (Keyboard::isKeyPressed(Keyboard::C))
        {
            input.value += 'C';
        }
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            input.value += 'D';
        }
    }
    if (!bloc[index / size][index % size])
    {
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            input.val = input.value[0] - 'A' + 1;

            bool correct;

            correct = checkColumn(matrix, size, index % size, input.val) && checkRow(matrix, size, index / size, input.val) && checkSquare(matrix, size, index / size, index % size, input.val);

            if (input.val > 0 && input.val <= size && correct)
            {
                matrix[index / size][index % size] = input.val;
                input.value = "";
            }
            else
            {
                input.value = "";
            }
        }
    }
}

// Перетворення цілого числа в рядок
string intTOstring(int number)
{
    if (number == 0)
        return "0";
    string temp = "";
    string returnvalue = "";
    while (number > 0)
    {
        temp += number % 10 + 48;
        number /= 10;
    }
    for (int i = 0; i < temp.length(); i++)
    {
        returnvalue += temp[temp.length() - i - 1];
    }
    return returnvalue;
}

// Малювання квадратів та заповнення їх значеннями
void Engine::drawSquare(RenderWindow& window, int** matrix, bool** bloc, int size, int index)
{
    int quadx = 2, quady = 2;
    int cellSize = 100, fontSize = 50;

    Font font;
    font.loadFromFile("Raleway-Black.ttf");

    RectangleShape mediumSquare;
    mediumSquare.setSize(Vector2f(cellSize * quadx, cellSize * quady));
    mediumSquare.setOutlineThickness(2);
    mediumSquare.setOutlineColor(sf::Color::Black);
    mediumSquare.setFillColor(sf::Color::Transparent);

    RectangleShape bigSquare;
    bigSquare.setSize(Vector2f(cellSize * size, cellSize * size));
    bigSquare.setOutlineThickness(4);
    bigSquare.setOutlineColor(sf::Color::Black);
    bigSquare.setFillColor(sf::Color::Transparent);
    bigSquare.setPosition((400 - size / 2 * cellSize), (300 - size / 2 * cellSize));

    RectangleShape smallSquare;
    smallSquare.setSize(Vector2f(cellSize, cellSize));
    smallSquare.setOutlineThickness(1);
    smallSquare.setOutlineColor(Color(200, 200, 200));
    smallSquare.setFillColor(sf::Color::Transparent);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            smallSquare.setPosition((400 - size / 2 * cellSize) + j * cellSize, (300 - size / 2 * cellSize) + i * cellSize);
            window.draw(smallSquare);
            string text;
            if (matrix[i][j] > 0)
            {
                text = 'A' + matrix[i][j] - 1;
            }
            else
            {
                text = "";
            }
            if (index % size == j && index / size == i)
            {
                smallSquare.setFillColor(Color(0, 255, 0, 210));
                smallSquare.setPosition((400 - size / 2 * cellSize) + j * cellSize, (300 - size / 2 * cellSize) + i * cellSize);
                window.draw(smallSquare);
                smallSquare.setFillColor(sf::Color::Transparent);
            }

            Text cell(text, font, fontSize);
            cell.setPosition((400 - size / 2 * cellSize) + j * cellSize + fontSize / 2, (300 - size / 2 * cellSize) + i * cellSize + fontSize / 2);
            if (bloc[i][j])
            {
                cell.setFillColor(Color(80, 80, 80));
            }
            else
            {
                cell.setFillColor(Color::White);
            }

            window.draw(cell);
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i % quady == 0 && j % quadx == 0)
            {
                mediumSquare.setPosition((400 - size / 2 * cellSize) + j * cellSize, (300 - size / 2 * cellSize) + i * cellSize);
                window.draw(mediumSquare);
            }
        }
    }
    window.draw(bigSquare);
}

// Заповнення таблиці значеннями
void Engine::fillTable(int** m, int& size, RenderWindow& window)
{
    int val[16];
    bool valbool[16];
    for (int i = 0; i < 16; i++)
    {
        val[i] = i + 1;
        valbool[i] = true;
    }

    int valueRow = (rand() % size);
    int reset = 0;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (valbool[valueRow])
            {
                bool check;

                check = checkRow(m, size, i, val[valueRow]) && checkColumn(m, size, j, val[valueRow]) && checkSquare(m, size, i, j, val[valueRow]);

                if (check)
                {
                    m[i][j] = val[valueRow];
                    for (int i = 0; i < 16; i++)
                    {
                        valbool[i] = true;
                    }
                    valueRow = (rand() % size);
                }
                else
                {
                    valbool[valueRow] = false;
                    j--;
                    valueRow = (valueRow + 1) % size;
                }
            }
            else
            {
                valueRow = (valueRow + 1) % size;
                j--;
                if (removing(valbool, size))
                {
                    for (int x = 0; x < 16; x++)
                    {
                        valbool[x] = true;
                        m[i][x] = 0;
                    }
                    reset++;
                    j = -1;
                    if (reset > 1000)
                    {
                        i = -1;
                        j = -1;
                        reset = 0;
                        for (int i2 = 0; i < size; i2++)
                        {
                            for (int j2 = 0; j2 < size; j2++)
                            {
                                m[i2][j2] = 0;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}

// Встановлення рівня складності гри
void Engine::setLevel(int** m, bool** b, int size, int difficulty)
{
    int amount;
    switch (difficulty)
    {
    case 0:
        amount = 2;
        break;
    case 1:
        amount = 3;
        break;
    case 2:
        amount = 5;
        break;
    default:
        amount = 2;
        break;
    }
    int numberCells = (size * size);
    int left = numberCells / amount;
    int deleted = numberCells - left;
    int row = rand() % size;
    int column = rand() % size;
    for (int i = 0; i < deleted; i++)
    {
        if (m[row][column] > 0)
        {
            m[row][column] = 0;
            b[row][column] = false;
        }
        else
        {
            row = rand() % size;
            column = rand() % size;
            i--;
        }
    }
}

// Перевірка чи виграв гравець
bool Engine::checkWin(int** m, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (m[i][j] == 0)
            {
                return false;
            }
        }
    }
    return true;
}
