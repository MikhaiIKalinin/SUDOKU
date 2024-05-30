#include "Game.h"

RenderWindow window(VideoMode(800, 700), "Sudoku", sf::Style::Titlebar | sf::Style::Close);


Game::Game()
{
    state = END;

    if (!font.loadFromFile("Raleway-Black.ttf"))
        return;

    state = MENU;
}


Game::~Game(void)
{
}


void Game::runGame()
{
    while (state != END)
    {
        switch (state)
        {
        case GameState::MENU:
            menu();
            break;
        case GameState::RULES:
            rules();
            break;
        case GameState::GAME:
            levels();
            break;
        }
    }
}


void Game::menu()
{
    Text title("Sudoku", font, 80);
    title.setStyle(Text::Bold);
    title.setPosition(800 / 2 - title.getGlobalBounds().width / 2, 60);

    const int ile = 3;

    Text tekst[ile];
    string str[] = { "Play", "Rules", "Exit" };

    for (int i = 0; i < ile; i++)
    {
        tekst[i].setFont(font);
        tekst[i].setCharacterSize(65);
        tekst[i].setString(str[i]);
        tekst[i].setPosition(800 / 2 - tekst[i].getGlobalBounds().width / 2, 200 + i * 120);
        tekst[i].setFillColor(Color::White); // ”становите цвет текста по умолчанию
    }

    while (state == MENU)
    {
        Vector2f mouse(Mouse::getPosition(window));
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
                state = END;

            else if (tekst[0].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
                state = GAME;

            else if (tekst[1].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
                state = RULES;

            else if (tekst[2].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
                state = END;
        }

        for (int i = 0; i < ile; i++)
        {
            if (tekst[i].getGlobalBounds().contains(mouse))
                tekst[i].setFillColor(Color::Red); // »змените цвет текста при наведении
            else
                tekst[i].setFillColor(Color::White); // ÷вет текста по умолчанию
        }

        window.clear();
        window.draw(title);
        for (int i = 0; i < ile; i++)
            window.draw(tekst[i]);
        window.display();
    }
}

void Game::levels()
{
    sf::Text title("Levels", font, 90);
    title.setStyle(sf::Text::Bold);
    title.setPosition(800 / 2 - title.getGlobalBounds().width / 2, 30);

    const int ile = 4;
    sf::Text text[ile];
    std::string aut[] = { "Easy", "Medium", "Hard", "back" };

    for (int i = 0; i < ile; i++)
    {
        text[i].setFont(font);
        text[i].setCharacterSize(65);
        text[i].setString(aut[i]);
        text[i].setPosition(800 / 2 - text[i].getGlobalBounds().width / 2, 150 + i * 120);
        text[i].setFillColor(sf::Color::White); // ”становите цвет текста по умолчанию
    }

    while (state == GAME && window.isOpen())
    {
        sf::Vector2f mouse(sf::Mouse::getPosition(window));
        sf::Event event;
        Engine engine;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (text[0].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
            {
                if (engine.runEngine(window, 0))
                    state = GAME;
                else
                    state = MENU;
            }
            else if (text[1].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
            {
                if (engine.runEngine(window, 1))
                    state = GAME;
                else
                    state = MENU;
            }
            else if (text[2].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
            {
                if (engine.runEngine(window, 2))
                    state = GAME;
                else
                    state = MENU;
            }
            else if (text[3].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
            {
                state = MENU;
            }
        }

        for (int i = 0; i < ile; i++)
        {
            if (text[i].getGlobalBounds().contains(mouse))
                text[i].setFillColor(sf::Color::Red); // »змените цвет текста при наведении
            else
                text[i].setFillColor(sf::Color::White); // ÷вет текста по умолчанию
        }

        window.clear();
        window.draw(title);
        for (int i = 0; i < ile; i++)
            window.draw(text[i]);
        window.display();
    }
}

void Game::rules()
{
    sf::Text title("Rules", font, 90);
    title.setStyle(sf::Text::Bold);
    title.setPosition(800 / 2 - title.getGlobalBounds().width / 2, 60);

    sf::Text description("KV-33 Kalinin Mykhailo\n"
        "Fill in the grid with numbers from A to D.\n"
        "In each row, each column, \neach digit must appear only once.\n"
        "Use the keyboard arrows to \nchange position, set the letters A, B, C, D.\n"
        "Have a good game.", font, 30);
    description.setPosition(800 / 2 - description.getGlobalBounds().width / 2, 180);
    description.setFillColor(sf::Color::White);

    const int ile = 1;

    sf::Text text[ile];

    std::string rul[] = { "Back" };
    for (int i = 0; i < ile; i++)
    {
        text[i].setFont(font);
        text[i].setCharacterSize(65);
        text[i].setString(rul[i]);
        text[i].setPosition(800 / 2 - text[i].getGlobalBounds().width / 2, 450 + i * 120);
    }

    while (state == RULES && window.isOpen())
    {
        sf::Vector2f mouse(sf::Mouse::getPosition(window));
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (text[0].getGlobalBounds().contains(mouse) && event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
            {
                state = MENU;
            }
        }

        for (int i = 0; i < ile; i++)
        {
            if (text[i].getGlobalBounds().contains(mouse))
                text[i].setFillColor(Color::Red);
            else
                text[i].setFillColor(Color::White);
        }

        window.clear();
        window.draw(title);
        window.draw(description);
        for (int i = 0; i < ile; i++)
            window.draw(text[i]);
        window.display();
    }
}