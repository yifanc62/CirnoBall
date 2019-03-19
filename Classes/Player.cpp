#include "Player.h"

Player::Player(int difficulty) {
	if (difficulty == 1) {
		life = 99;
	}
	else if (difficulty == 2) {
		life = 9;
	}
	else {
		life = 5;
	}
	score = 0;
	stage = 1;
	combo = 0;
}
int Player::getLife() {
	return life;
}
void Player::addOneLife() {
	life += 1;
}
void Player::minusOneLife(){
	life -= 1;
}
int Player::getStage(){
	return stage;
}
void Player::addOneStage(){
	stage += 1;
}
void Player::minusOneStage(){
	stage -= 1;
}
void Player::setStage(int _stage){
	stage = _stage;
}
int Player::getCombo(){
	return combo;
}
void Player::addOneCombo(){
	combo += 1;
}
void Player::emptyCombo(){
	combo = 0;
}
long Player::getScore(){
	return score;
}
void Player::addScore(long _score){
	score += _score;
}
void Player::emptyScore(){
	score = 0;
}