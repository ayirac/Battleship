#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace std;
using namespace sf;

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(1500, 1250), "Battleship");
    window.setVerticalSyncEnabled(true);
    Game battleship(&window);

    while (window.isOpen())
    {
        window.clear();
        // Draw a the current screen
    	if (battleship.get_state() == 0)
    		battleship.main_menu();
    	else if (battleship.get_state() == 1)
    		battleship.ship_menu();
    	else if (battleship.get_state() == 2)
    		battleship.game_start();
        else if (battleship.get_state() == 3)
            battleship.post_game();

        sf::Event event;
    	while (window.pollEvent(event))
    	{
            // get mos pos & convert to worldpos
            sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // General keybinds
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            // Main menu keybinds
            if (battleship.get_state() == 0)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    battleship.process_click(mouse_pos);
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (battleship.holding_button())
                        battleship.release_button();
                }
            }
            // Ship setup menu keybinds
            else if (battleship.get_state() == 1)
            {

                if (event.type == sf::Event::MouseButtonPressed)
                {
                    battleship.process_click(mouse_pos);
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (battleship.holding_figurine())
						battleship.release_figurine(mouse_pos);
                    if (battleship.holding_button())
                        battleship.release_button();
                }
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == Keyboard::A || event.key.code == Keyboard::D)
                    {
                        if (battleship.holding_figurine())
                            battleship.rotate_held_figurine(event.key.code);
                    }
                }
            }
            // Singleplayer battle keybinds
            else if (battleship.get_state() == 2)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    battleship.process_click(mouse_pos);
                    if (battleship.holding_figurine())
                    {
                        bool successful_player_attack = false, successful_enemy_attack = false;
                        sf::Vector2i player_attack_pos, enemy_attack_pos;
                        if (battleship.process_hit(mouse_pos, false, player_attack_pos, successful_player_attack))
                        {
                        	battleship.process_ai_hit(enemy_attack_pos, successful_enemy_attack);
                            battleship.get_statistics().add_entry(player_attack_pos, enemy_attack_pos, successful_player_attack, successful_enemy_attack);
                        }
                    }
                     battleship.get_statistics().process_click(mouse_pos);
                    
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (battleship.holding_figurine())
                        battleship.release_figurine();
                    else if (battleship.holding_button())
                        battleship.release_button();
                    else if (battleship.get_statistics().holding_button())
                        battleship.get_statistics().release_button();
                }
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == Keyboard::Space)
                    {
                        if (battleship.holding_figurine())
                            battleship.release_figurine();
                        else
                            battleship.ready_attack();
                    }
                }
            }
            else if (battleship.get_state() == 3) // post-game keybinds
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    battleship.get_statistics().process_click(mouse_pos);
                    battleship.process_click(mouse_pos);
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                	if (battleship.holding_button())    
                        battleship.release_button();
                	else if (battleship.get_statistics().holding_button())
                        battleship.get_statistics().release_button();
                }
            }

        }
        window.display();
    }
    return 0;
}