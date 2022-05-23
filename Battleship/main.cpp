#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace std;
using namespace sf;

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width * 0.586, sf::VideoMode::getDesktopMode().height * 0.868), "Battleship");
    window.setPosition(sf::Vector2i((VideoMode::getDesktopMode().width - window.getSize().x )/ 2, (VideoMode::getDesktopMode().height - window.getSize().y) /2));
    window.setVerticalSyncEnabled(true);
    Game battleship(&window);
    sf::Clock clock;
    double elapsed = 0;

    while (window.isOpen())
    {
        // debug check for host connection
        if (!battleship.get_multiplayer().get_listener().Disconnected)
	        std::cout << "Hosting!\n";
        window.clear();

        // State binding to respective draw functions
        if (battleship.get_multiplayer().connected_init() && battleship.get_state() != 5)
        {
            battleship.get_multiplayer().disable_connected_init();
            battleship.set_state(5);
            battleship.set_chatbox_state(1);
        }
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
        else if (battleship.get_state() == 5)
            battleship.multiplayer_ship_menu();
        else if (battleship.get_state() == 6)
            battleship.multiplayer_game_start();


        // Event/keybind handling for each stages
        sf::Event event;
    	while (window.pollEvent(event))
    	{
            // get mos pos & convert to worldpos
            sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            // General keybinds
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::End)
                    window.close();

            // General events
            if (event.type == sf::Event::Closed)
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
                    battleship.process_popup_box(mouse_pos);
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (battleship.holding_figurine())
						battleship.release_figurine(mouse_pos);
                    if (battleship.holding_button())
                        battleship.release_button();
                    battleship.check_popup_boxes_exit();
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
                        if (battleship.process_hit(mouse_pos, player_attack_pos, successful_player_attack))
                        {
                            battleship.process_ai_hit(enemy_attack_pos, successful_enemy_attack);
                            battleship.get_statistics().add_entry(player_attack_pos, enemy_attack_pos, successful_player_attack, successful_enemy_attack);
                        }
                    }
                     battleship.get_statistics().process_click(mouse_pos);
                     battleship.process_popup_box(mouse_pos);
                    
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (battleship.holding_figurine())
                        battleship.release_figurine();
                    else if (battleship.holding_button())
                        battleship.release_button();
                    else if (battleship.get_statistics().holding_button())
                        battleship.get_statistics().release_button();
                    battleship.check_popup_boxes_exit();
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
                    if (battleship.get_chatbox()->get_state() == 1 && battleship.get_multiplayer().connected())
                        battleship.get_chatbox()->process_click(mouse_pos);
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                	if (battleship.holding_button())    
                        battleship.release_button();
                	else if (battleship.get_statistics().holding_button())
                        battleship.get_statistics().release_button();

                    if (battleship.get_multiplayer().connected()) // Chatbox keybinds for multiplayer
                    {
                        if (battleship.get_chatbox()->edit_mode() && !battleship.get_chatbox()->contains(mouse_pos))
                            battleship.get_chatbox()->set_edit_mode(false);
                            if (battleship.get_chatbox()->holding_button())
                                battleship.get_chatbox()->release_button();
                    }
                }
                if (battleship.get_chatbox()->edit_mode() && battleship.get_multiplayer().connected()) // checks for user input for the text input box when its in edit mode
                {
                    if (event.type == sf::Event::KeyReleased)
                    {
                        if (event.key.code == sf::Keyboard::Escape)
                            battleship.get_chatbox()->set_edit_mode(false);
                    }
                    else if (event.key.code == sf::Keyboard::Enter && battleship.get_multiplayer().get_connecting() == nullptr)
                    {
                        battleship.get_chatbox()->enter_key_pressed();
                    }
                    else if (event.type == sf::Event::TextEntered)
                    {
                        if (event.text.unicode == '\b')
                        {
                            if (battleship.get_chatbox()->get_textfield_size() > 0)
                                battleship.get_chatbox()->delete_end_textfield();
                        }
                        else if (event.text.unicode < 128)
                        {
                            std::cout << "ASCII CHAR: " << static_cast<char>(event.text.unicode) << std::endl;
                            battleship.get_chatbox()->process_keyboard(static_cast<char>(event.text.unicode));
                        }
                    }
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
                    battleship.process_popup_box(mouse_pos);
                }

                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (battleship.holding_button())
                        battleship.release_button();
                    if (battleship.get_inputbox()->edit_mode() && !battleship.get_inputbox()->contains(mouse_pos))
                        battleship.get_inputbox()->set_edit_mode(false);
                    if (battleship.get_inputbox()->holding_button())
                        battleship.get_inputbox()->release_button();
                    battleship.check_popup_boxes_exit();
                }

                if (battleship.get_inputbox()->edit_mode()) // checks for user input for the text input box when its in edit mode
                {
                    if (event.key.code == sf::Keyboard::Escape)
                        battleship.get_inputbox()->set_edit_mode(false);
                    else if (event.key.code == sf::Keyboard::Enter && battleship.get_multiplayer().get_connecting() == nullptr)
                    {
                        battleship.get_inputbox()->set_state(1);
                        battleship.get_multiplayer().connect(sf::IpAddress(battleship.get_inputbox()->get_textfield_entry()));
                    }
                    else if (event.type == sf::Event::KeyPressed)
                    {
                        if (event.key.control && event.key.code == sf::Keyboard::V)
                        {
                            for (int i = 0; i < sf::Clipboard::getString().getSize(); i++)
                                battleship.get_inputbox()->process_keyboard(sf::Clipboard::getString()[i]);
                        }
                    }
                    else if (event.type == sf::Event::TextEntered)
                    {
                        if (event.text.unicode == '\b')
                        {
                            if (battleship.get_inputbox()->get_tip_status())
                            {
                                for (int i = 0; i < battleship.get_inputbox()->get_textfield_entry().size(); i++)
									battleship.get_inputbox()->delete_end_textfield();
                            }
                                
                            if (battleship.get_inputbox()->get_textfield_size() > 0)
								battleship.get_inputbox()->delete_end_textfield();
                        }
                        else if (event.text.unicode > 31 && event.text.unicode < 128)
                        {
                            std::cout << "ASCII CHAR: " << static_cast<char>(event.text.unicode) << std::endl;
                            battleship.get_inputbox()->process_keyboard(static_cast<char>(event.text.unicode));
                        }
                    }
                }
            }
            else if (battleship.get_state() == 5)
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    battleship.process_click(mouse_pos);
                    if (battleship.get_chatbox()->get_state() == 1)
                        battleship.get_chatbox()->process_click(mouse_pos);
                    battleship.process_popup_box(mouse_pos);
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (battleship.holding_button())
                        battleship.release_button();
                    if (battleship.get_chatbox()->edit_mode() && !battleship.get_chatbox()->contains(mouse_pos))
                        battleship.get_chatbox()->set_edit_mode(false);
                    if (battleship.get_chatbox()->holding_button())
                        battleship.get_chatbox()->release_button();
                    if (battleship.holding_figurine())
                        battleship.release_figurine(mouse_pos);
                    battleship.check_popup_boxes_exit();
                }
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == Keyboard::A || event.key.code == Keyboard::D)
                    {
                        if (battleship.holding_figurine())
                            battleship.rotate_held_figurine(event.key.code);
                    }
                }
                if (battleship.get_chatbox()->edit_mode()) // checks for user input for the text input box when its in edit mode
                {
                    if (event.type == sf::Event::KeyReleased)
                    {
                        if (event.key.code == sf::Keyboard::Escape)
                            battleship.get_chatbox()->set_edit_mode(false);
                    }
                    else if (event.key.code == sf::Keyboard::Enter && battleship.get_multiplayer().get_connecting() == nullptr)
                    {
                        battleship.get_chatbox()->enter_key_pressed();
                    }
                    else if (event.type == sf::Event::TextEntered)
                    {
                        if (event.text.unicode == '\b')
                        {
                            if (battleship.get_chatbox()->get_textfield_size() > 0)
								battleship.get_chatbox()->delete_end_textfield();
                        }
                        else if (event.text.unicode < 128)
                        {
                            std::cout << "ASCII CHAR: " << static_cast<char>(event.text.unicode) << std::endl;
                            battleship.get_chatbox()->process_keyboard(static_cast<char>(event.text.unicode));
                        }
                    }
                }
            }
            else if (battleship.get_state() == 6)
            {
	            if (event.type == sf::Event::MouseButtonPressed)
	            {
                    battleship.process_click(mouse_pos);
	                if (battleship.get_chatbox()->get_state() == 1)
	                    battleship.get_chatbox()->process_click(mouse_pos);
                    battleship.get_statistics().process_click(mouse_pos);
	                battleship.process_popup_box(mouse_pos);

                    if (battleship.holding_figurine())
                    {
                        bool successful_player_attack = false, successful_enemy_attack = false;
                        sf::Vector2i player_attack_pos, enemy_attack_pos;
                        if (battleship.process_hit(mouse_pos, player_attack_pos, successful_player_attack))
                        {
                            battleship.set_turn(false);
                            battleship.send_attack_data(player_attack_pos, successful_player_attack);
                            battleship.get_statistics().add_entry(player_attack_pos, successful_player_attack, true);
                            battleship.update_round();
                        }
                    }
	            }
	            else if (event.type == sf::Event::MouseButtonReleased)
	            {
                    if (battleship.holding_figurine())
                        battleship.release_figurine();
	                else if (battleship.holding_button())
	                    battleship.release_button();
                    else if (battleship.get_statistics().holding_button())
                        battleship.get_statistics().release_button();
	                if (battleship.get_chatbox()->edit_mode() && !battleship.get_chatbox()->contains(mouse_pos))
	                    battleship.get_chatbox()->set_edit_mode(false);
	                if (battleship.get_chatbox()->holding_button())
	                    battleship.get_chatbox()->release_button();
	                battleship.check_popup_boxes_exit();
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
	            if (battleship.get_chatbox()->edit_mode()) // checks for user input for the text input box when its in edit mode
	            {
	                if (event.type == sf::Event::KeyReleased)
	                {
	                    if (event.key.code == sf::Keyboard::Escape)
	                        battleship.get_chatbox()->set_edit_mode(false);
	                }
	                else if (event.key.code == sf::Keyboard::Enter && battleship.get_multiplayer().get_connecting() == nullptr)
	                {
	                    battleship.get_chatbox()->enter_key_pressed();
	                }
	                else if (event.type == sf::Event::TextEntered)
	                {
	                    if (event.text.unicode == '\b')
	                    {
	                        if (battleship.get_chatbox()->get_textfield_size() > 0)
	                            battleship.get_chatbox()->delete_end_textfield();
	                    }
	                    else if (event.text.unicode < 128)
	                    {
	                        std::cout << "ASCII CHAR: " << static_cast<char>(event.text.unicode) << std::endl;
	                        battleship.get_chatbox()->process_keyboard(static_cast<char>(event.text.unicode));
	                    }
	                }
	            }
            }
        }

        // Draw animations
        sf::Time dt = clock.restart();
        elapsed += dt.asSeconds();
        if (battleship.get_multiplayer().get_connecting() != nullptr)
        {
            battleship.get_animation(0).update(elapsed);
            window.draw(*battleship.get_sprite(0));
        }
        if (elapsed > 1)
            elapsed = 0;

        window.display();
    }
    return 0;
}