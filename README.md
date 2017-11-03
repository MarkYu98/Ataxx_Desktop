# Ataxx_Desktop
This is a desktop version of Ataxx(同化棋) game GUI and AI developed by Qt cross-platform framework.
Have fun!

## About the Project
This is a assignment project for my Introduction to Computing class at Peking University. (Although the original requirement for this assignment does not include a GUI and multiple level of AIs) This version provides 4 levels of AI to play with (Of course man-to-man game is also available).

Available for Windows/macOS (compiled and tested), should be able to transplant to iOS, Android, etc.

Provides Save/Load function and some quite interesting GUI custom settings.

A web version of the game is under development.

The AI of the game can also be found on [Botzone.org](www.botzone.org). My ID on Botzone is MarkYu98.

Different levels of AI algorithms include random, greedy, Min-Max search.

There are also quite a lot of outstanding AIs of Ataxx made by my classmates on [Botzone.org](www.botzone.org).

## 游戏规则 ([Botzone Wiki](http://wiki.botzone.org/index.php?title=Ataxx))
这里的同化棋游戏与国际通行的同化棋规则基本一致。

同化棋是一种二人棋盘游戏，黑白双方交替在7x7的棋盘上移动棋子。
玩家每次可以移动的位置是有限的，具体步骤解释如下：

选取现在在棋盘上己方的一枚棋子，再选择一个落子位置。要求落子处为空，既没有对方也没有己方的棋子。
选择的落子位置可以是以下二者之一：

**落子位置在以选取的棋子为中心的3x3的范围内。此时选取的棋子会复制自身到落子位置。一共有8个位置可以选择。**

**落子位置在以选取的棋子为中心的3x3的范围外、5x5的范围内，此时选取的棋子会移动自身到落子位置，一共有16个位置可以选择。**

总的来说，移动一格是“复制粘贴”，移动两格是“剪切粘贴”。

**每次落子后，新位置的落子会将落子处紧邻的8个位置的棋子（如果有）都变成自己的棋子。**

若没有上述这种位置则游戏结束，将所有无子的地方算作有子可下一方的棋子，然后数出双方棋子数，棋多者胜。

任意时刻对方棋子数为0时，己方胜利。

游戏回合数到400时，游戏会直接结束，数出双方棋子数，棋多者胜。

## Contact Me
Email: markyu@pku.edu.cn; markyu98@outlook.com
