*THIS DOCUMENT CONTAINS INSTRUCTURES ON HOW TO MAKE A CUSTOM LEVEL*

-Levels are made in a text documents and when reading them in the game make sure you include the
 .TXT at the end as it won't load with it.

 -Each level has a max amount to go on the X and Y and this is displayed at the top of the document like this:
 20 20

 -Each level will need to be layed out like this:
 11111
 10001
 10001
 10001
 11111

 -So a full example would look like this:
 5 5
 11111
 10001
 10001
 10001
 11111
 Which will load a 5 by 5 maze.

 -The level will read down from the top left and then when it hits the bottom it moves to the right and resets at the top

 -Each level has a min requirements to be met and these requirements are:
 1 Player spawner but can have more
 1 End point but can have more
 1 AI Spawner but can have more

 Here are the number indexs and what each means:
 0 - Empty Space
 1 - Walls
 2 - Player Start Point (Will use the last one to be read in)
 3 - The End Point (Will use the last one to be read in)
 4 - AI Spawners
 6 - Torches (Usally placed on the side of walls)
 7 - Statues (Usally placed on the side of doors)
 8 - Room Props
 9 - Doors in the walls

 Prop Requirements:
 Torches will only spawns on walls
 Status will only spawn if they aren't in a one gap corrider, Example:
 111111111111111111111111
 000000000000000000000000
 111111111111111111111111

 Props will anywhere where they are atleast one spawn away from a wall.

 - Thats about it, go make some level!
