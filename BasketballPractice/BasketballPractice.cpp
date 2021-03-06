// BasketballPractice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include<iostream>
#include <sstream>
#include<numeric>
#include <iterator>
#include <windows.h>

using namespace std;
enum Gymn
{
	EMPTY,
	GROUND,
	HOOP
};
enum Player
{
	NONE,
	HEAD,
	BODY,
	LEGS
};
struct Position {
	int x = 0;
	int y = 0;
};
struct Velocity
{
	int x = 0;
	int y = 0;
};

void makeGame(vector<vector<bool>>&, vector<vector<Gymn>>&, vector<vector<Player>> &, int, int);
void intro(vector<vector<bool>>&, vector<vector<Gymn>>&, vector<vector<Player>> &, int, int);
void showGame(vector<vector<bool>>&, vector<vector<Gymn>>&, vector<vector<Player>> &);
void gravity(Velocity&);
void game();
void inertia(Position&, Velocity&, vector<vector<bool>> &, int);
void showBall(Position&, vector<vector<bool>> &);
void wipe(vector<vector<bool>>&);
void drag(Velocity &, int &, int &);
Velocity getCoordinates(string, vector<vector<bool>>&);
bool scored(int, int, Position &, Position &);
int main()
{

	game();

	return 0;
}
void game()
{
	vector<Player> scan_line_player(30, Player::NONE);
	vector<Gymn>scan_line_gymn(30, Gymn::EMPTY);
	vector<bool> scan_line_ball(30, false);
	vector<vector<Player>>player_screen(30, scan_line_player);
	vector<vector<Gymn>>gymn_screen(30, scan_line_gymn);
	vector<vector<bool>>ball_screen(30, scan_line_ball);
	cout << "Basketball Practise\n\nAlex Mair";
	Sleep(1000);
	system("CLS");
	intro(ball_screen, gymn_screen, player_screen, 15, 15);
	char isReady;
	cin >> isReady;
	switch (isReady)
	{
	case 'y':
		break;
	case 'n':
		cout << "You're a goddam quitter" << endl;
		return;
	default:
		break;
	}
	showGame(ball_screen, gymn_screen, player_screen);

	vector<bool> score;
	int prev_score = 0;
	int misses = 0;
	string msg = "";
	while (true)
	{
		Position p{ 0, ball_screen.size() - 4 };
		bool has_scored = false;

		wipe(ball_screen);
		makeGame(ball_screen, gymn_screen, player_screen, 15, 15);
		showGame(ball_screen, gymn_screen, player_screen);



		Velocity v{ 0,0 };
		
			for (size_t i = 0; i < score.size(); i++)
			{
				if (score.at(i))
				{
					cout << "(|)";
				}
				else
				{
					cout << "X";
				}
			}
			cout << endl;
		
		cout << msg;
		cout << "Enter v_x" << endl;
		while (true)
		{
			cin >> v.x;
			if (cin)
			{
				break;

			}
			else {
				cin.clear();
				cin.ignore();
				cout << "v_x must be an integer. Please try again" << endl;

			}
		}
		cout << "Enter v_y" << endl;
		while (true)
		{


			int v_y;
			cin >> v_y;
			v.y = -v_y;
			if (cin)
			{
				break;

			}
			else {
				cin.clear();
				cin.ignore();
				cout << "v_y must be an integer. Please try again" << endl;
			}
		}
		msg = "";
		
		for(int ticks = 1; ticks < 50; ticks++)
		{

			Position prev_p = p;
			

			showGame(ball_screen, gymn_screen, player_screen);
			wipe(ball_screen);
			inertia(p, v, ball_screen, 1);
			gravity(v);
			if (scored(15, 15, prev_p, p))
			{
				has_scored = true;
				score.push_back(true);
				int j = 0;
				if (score.size() == 1 && score.front())
				{
					msg = "Got one!\n";
				}
				if (score.size() > 1)
				{
					do
					{
						j++;
					} while (score.at(score.size() - j)
						&& j != score.size());
				}

				if (j-1 == 1)
				{
					msg = "Got one!\n";
				}
				if (j-1 == 2)
				{
					msg = "Keep going!\n";
				}
				if (j-1 == 3)
				{
					msg = "You're on a roll!\n";
				}
			}
			if (ticks > 60)
			{
				drag(v, p.x, ticks);
			}
			else if (ticks % 3 == 0)
			{
				drag(v, p.x, ticks);

			}


			showBall(p, ball_screen);
			system("CLS");
		}
		if (!has_scored)
		{
			score.push_back(false);
			int j = 0;
			if (score.size() == 1 && score.front())
			{
				msg = "keep trying\n";
			}
			if (score.size() > 1)
			{
				do
				{
					j++;
				}while (!score.at(score.size() - j)
					&& score.size()!= j);
			}
			if (j-1 == 1)
			{
				msg = "keep trying\n";
			}
			if (j-1 == 2)
			{
				msg = "third time lucky?\n";
			}
		}
	}
}
void gravity(Velocity &v)
{
	v.y += 1;
}
void introGravity(Velocity &v)
{
	v.y += 3;
}
void inertia(Position &p, Velocity &v, vector<vector<bool>> &ball_screen, int bounce_factor)
{
	if (p.x + v.x<0 || p.x + v.x>ball_screen.size() - 1)
	{
		v.x = -v.x;
	}
	if (p.y + v.y > 29)
	{
		v.y = bounce_factor - v.y;

	}
	p.x += v.x;
	p.y += v.y;

}
void showBall(Position &p, vector<vector<bool>> &ball_screen)
{
	for (size_t i = 0; i < ball_screen.size(); i++)
	{
		for (size_t j = 0; j < ball_screen.size(); j++)
		{
			if (p.x == j && p.y == i)
			{
				ball_screen[i][j] = true;
			}
		}
	}
}
void makeGame(vector<vector<bool>> &ball_screen, vector<vector<Gymn>> &gymn_screen, vector<vector<Player>> &player_screen, int cross_shift, int height)
{
	ball_screen[ball_screen.size() - 4][1] = true;
	for (size_t i = 0; i < gymn_screen.size(); i++)
	{
		gymn_screen[gymn_screen.size() - 1].at(i) = Gymn::GROUND;
	}
	gymn_screen[height][cross_shift + 0] = Gymn::HOOP;
	gymn_screen[height][cross_shift + 1] = Gymn::HOOP;
	gymn_screen[height][cross_shift + 2] = Gymn::HOOP;
	gymn_screen[height][cross_shift + 3] = Gymn::HOOP;

	for (size_t i = 0; i < gymn_screen.size(); i++)
	{
		gymn_screen[i].at(0);
	}
	player_screen[player_screen.size() - 3][0] = Player::HEAD;
	player_screen[player_screen.size() - 2][0] = Player::BODY;
	player_screen[player_screen.size() - 1][0] = Player::LEGS;
}

void intro(vector<vector<bool>> &ball_screen, vector<vector<Gymn>> &gymn_screen, vector<vector<Player>> &player_screen, int cross_shift, int height)
{
	
	for (size_t i = 0; i < gymn_screen.size(); i++)
	{
		gymn_screen[gymn_screen.size() - 1].at(i) = Gymn::GROUND;
	}
	Position p{ 15, 0 };
	Velocity v{ 0, 0 };
	int ticks = 0;
	for (size_t i = 0; i < 12; i++)
	{
		showGame(ball_screen, gymn_screen, player_screen);
		wipe(ball_screen);
		
			inertia(p, v, ball_screen, 4);
			introGravity(v);
		
		showBall(p, ball_screen);
		system("CLS");
	}
	cout << "I'm LeBron Kenobi and I'm here to teach you to play" << endl;
	cout << "goddam basketball, goddamit!\n" << endl;
	cout << "The rules are goddam simple. You've got to get that ball into the hoop.  You got ten tries"<<endl;
	cout << "If you get eight outta ten, you're on the goddam team.\n If you get less than five outta ten" << endl;
	cout << "then so help me god if you ever touch a basketball again because if there's one goddam thing I can't stand" << endl;
	cout << "it's losers" << endl;
	cout << "Are you ready to goddam rumble!?(y/n)";
}
void showGame(vector<vector<bool>> &ball_screen, vector<vector<Gymn>> &gymn_screen, vector<vector<Player>> &player_screen)
{
	for (size_t i = 0; i < gymn_screen.size(); i++)
	{
		for (size_t j = 0; j < gymn_screen.size(); j++)
		{

			switch (gymn_screen[i][j])
			{
			case Gymn::EMPTY:
				cout << " ";
				break;
			case Gymn::GROUND:
				cout << "_";
				break;
			case Gymn::HOOP:
				cout << "-";
				break;
			default:
				break;
			}
			switch (player_screen[i][j])
			{

			case Player::HEAD:
				cout << "\\o ";
				break;
			case Player::BODY:
				cout << " | ";
				break;
			case Player::LEGS:
				cout << "/\\";
				break;
			default:
				break;
			}

			if (ball_screen[i][j])
			{
				cout << "0";
			}
		}
		cout << endl;
	}
}
void wipe(vector<vector<bool>> &ball_screen)
{
	for (size_t i = 0; i < ball_screen.size(); i++)
	{
		for (size_t j = 0; j < ball_screen.size(); j++)
		{
			if (ball_screen[i][j])
			{
				ball_screen[i][j] = false;
			}
		}
	}
}

void drag(Velocity &v, int &p_x, int &ticks)
{

	if (v.x > 0)
	{
		v.x -= 1;
	}
	if (v.x < 0)
	{
		v.x += 1;
	}
	if (v.y > 0)
	{
		v.y -= 1;
	}
	if (v.y < 0)
	{
		v.y += 1;
	}
}
;
bool scored(int height, int cross_shift, Position &prev_p, Position &p)
{
	return (p.x >= height && prev_p.x <= height) && ((p.y >= 0 + cross_shift && prev_p.y <= 0 + cross_shift) || (p.y >= 1 + cross_shift && prev_p.y <= 1 + cross_shift)
		|| (p.y >= 2 + cross_shift && prev_p.y <= 2 + cross_shift)
		|| (p.y >= 3 + cross_shift && prev_p.y <= 3 + cross_shift)
		|| (p.y <= 0 + cross_shift && prev_p.y >= 0 + cross_shift) || (p.y <= 1 + cross_shift && prev_p.y >= 1 + cross_shift)
		|| (p.y <= 2 + cross_shift && prev_p.y >= 2 + cross_shift)
		|| (p.y <= 3 + cross_shift && prev_p.y >= 3 + cross_shift));
}



