# Kniwwelino-speed-game
As the name implies, a Kniwwelino-based speed test/game with multiple boards.

## The idea
The basic idea is a multiplayer speed test or game. Up to four people (well, technically 256 people) have their own Kniwwlino, on which they press a button after the main Kniwwelino does a countdown.

## Adding more Kniwwelinos to the project
The process of adding external Kniwwelinos to the project (for more players) is quite simple. __Note:__ as already said, only up 256 players are allowed to be used per game. So, to add more Kniwwelinos, one needs to connect the wanted Kniwwelino to a computer, modify the source code so that it contains the correct ID number (the first one is 1, the second is 2 and so on...) and upload it to the Kniwwlino either from Arduino IDE or some other equivalent interface. The ID number mentioned is stored and sent as an eight-bit (or one byte) unsigned integer, and this is where the 256 player limit comes from, as with eight bits the computer can only count up to 255 (including zero). Of course, this limit can be passed by increasing the byte count to two, which would move the limit to 65536 players. Again, if this is not enough, one could use four, or even eight bytes, and make the limit massive (though 256 players is probably sufficient enough).

## Technical information
It uses a protocol called [ESP-NOW](https://www.espressif.com/en/products/software/esp-now/ "ESP-NOW") to communicate between the Kniwwelinos. It uses similar frequencies as Wi-Fi (2.4 GHz) for that, but instead of connecting to a server, it directly broadcasts it to other Kniwwlinos, peer-to-peer. For this purpose, all Kniwwelinos in the game except the main one must know the main one's MAC-address.

When either the A or B button is pressed, if the Kniwwleino has not been assigned to be the main Kniwwelino, it will flash its LED blue for 10 milliseconds (just to indicate the user) and send its ID number as an unsigned byte to the main Kniwwelino. This way the main Kniwwelino knows which board's button was pressed first, and there is no need for bidirectional communication, as the only information being transmitted is _who_ pushed a button (not _when_ it was pushed).

The main Kniwwelino itself will display a countdown from five, and when it reaches zero it will flash red, green and blue lights on its LED and dislay a star-like image (shown below) on the matrix. In this state it is waiting for incoming messages about button presses on the other boards. Once it receives one, it will display the text "Player # wins!", where the '#' is replaced with the number it received (i.e. the player's ID). After scrolling the text, the Kniwwelino will wait for either the A or B button to be pressed. If the user does so, the game will restart and the countdown will start again.

Each Kniwwelino requires a power supply, for example a standard 5V power bank.

The star-like shape displayed on the matrix:

`
&#35; &#35; &#35;
 &#35;&#35;&#35;
&#35;&#35;&#35;&#35;&#35;
 &#35;&#35;&#35;
&#35; &#35; &#35;
`

