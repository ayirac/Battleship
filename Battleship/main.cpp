#include <iostream>
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
        // debug check for host connection
        if (!battleship.get_multiplayer().get_listener().Disconnected)
	        std::cout << "Hosting!\n";
        window.clear();
        // Draw a the current screen
    	if (battleship.get_state() == 0)
    		battleship.main_menu();
    	else if (battleship.get_state() == 1)
    		battleship.ship_menu();
    	else if (battleship.get_state() == 2)
    		battleship.singleplayer_game_start();
        else if (battleship.get_state() == 3)
            battleship.post_game();
        else if (battleship.get_state() == 4)
            battleship.multiplayer_setup();

        sf::Event event;
    	while (window.pollEvent(event))
    	{
            // get mos pos & convert to worldpos
            sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // General keybinds
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::End)
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
            // Multiplayer setup keybinds
            else if (battleship.get_state() == 4)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    battleship.process_click(mouse_pos);
                    if (battleship.get_inputbox()->get_state() == 1)
                        battleship.get_inputbox()->process_click(mouse_pos);
                }

                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (battleship.holding_button())
                        battleship.release_button();
                    if (battleship.get_inputbox()->edit_mode() && !battleship.get_inputbox()->contains(mouse_pos))
                        battleship.get_inputbox()->set_edit_mode(false);
                    if (battleship.get_inputbox()->holding_button())
                        battleship.get_inputbox()->release_button();
                }

                if (battleship.get_inputbox()->edit_mode()) // checks for user input for the text input box when its in edit mode
                {
                    if (event.key.code == sf::Keyboard::Escape)
                        battleship.get_inputbox()->set_edit_mode(false);
                    if (event.type == sf::Event::TextEntered)
                    {
                        if (event.text.unicode == '\b')
                        {
                            if (battleship.get_inputbox()->get_textfield_size() > 0)
								battleship.get_inputbox()->delete_end_textfield();
                        }
                        else if (event.text.unicode == '\n')
                        {
                            battleship.get_inputbox()->get_multiplayer()->connect(battleship.get_inputbox()->get_textfield_entry());
                        }
                        else if (event.text.unicode < 128)
                        {
                            std::cout << "ASCII CHAR: " << static_cast<char>(event.text.unicode) << std::endl;
                            battleship.get_inputbox()->process_keyboard(static_cast<char>(event.text.unicode));
                        }
                    }
                }
            }
        }
        window.display();
    }
    return 0;
}