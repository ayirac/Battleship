# Battleship :bomb:
The classic game of Battleship written in C++ utilizing the SFML library with singleplayer and multiplayer.
<p align="center">
  <img src="https://github.com/ayirac/Battleship/blob/master/summed.gif" width="80%">
</p>

## Features :sparkles:
- Play singleplayer against the computer, where it targets a random spot and then hunts for more hits in the same location
- Play multiplayer against another player by hosting a game yourself or connecting to an IP address, includes a chatbox!
- Place ships exactly where you want by rotating with 'WASD' and then drag and drop.
- Turn history and statistics UI modules that gives valuable information to a user in an easy to read format
- Easy to navigate GUI that can be modified with ease to implement new features

## Game Setup :wrench:
Players can move their ships exactly where they want by dragging and dropping and using 'WASD' to rotate orientations. 
<p align="center">
  <img src="https://github.com/ayirac/Battleship/blob/master/setup.gif" width="80%">
</p>

## Playing a Game :ship:
Two players face off in a race to sink eachothers ships before their own ships.
<p align="center">
  <img src="https://github.com/ayirac/Battleship/blob/master/game.gif" width="80%">
</p>

## Multiplayer :two:
Two players can play against eachother by hosting & connecting via IP address, features a chatbox as well for communcation.
<p align="center">
  <img src="https://github.com/ayirac/Battleship/blob/master/multi.png" width="80% height="80%">
</p>

## Challenges Faced
The first major challenge I faced was laying out the path to follow, which was difficult because there's alot of features/paths you can take. Another challenge that wasn't too difficult but it was tedious is fixing bugs related to ship placement to prevent crashes or misplacements required a lot of trial and error. The multiplayer aspect was an additional feature added on after the main game was done and setting up the commuication network for 2 clients to connect took some time to understand. Thankfully, SFML has good documentaiton and I was able to finish up the multiplayer module & even add a chatbox for the two players to talk with eachother.

## Motivation
I started working on this project after learning about OOP in depth in class because I wanted to get a feel for the strengths of objects. I may of gotten a little carried
away with the amount of classes my program has, as I made a class for every specific UI element. What I've realizied is that encapsulating each element in their own class
is that it is easy to modify the element without breaking another. I learned a great deal about OOP practices and how to approach similar problems with more concise code.
