struct Object
{
  int x;
  int y;
  int len;
};

void draw(int x, int y); // draw pixel at x, y
void drawObj(Object obj); // draw object at it's x, y
void controlPlayer(Object & player);
void controlBall(Object & ball);
void controlEnemy(Object & enemy);

const int ROWS = 8;
const int COLS = 8;
int rows[ROWS] = {7, 2, 11, 4, 19, 10, 18, 15};
int columns[COLS] = {3, 17, 16, 6, 12, 5, 1, 0};

int playerIn = 14; // potentiometer in
int rotat1 = 0, rotat2;

int ballVelX = -1;
int ballVelY = 1;
int ballTime1 = 0, ballTime2;
int ballTickSpeed = 160; // milliseconds

int enemyTime1 = 0, enemyTime2;
int enemyTickSpeed = 260; // milliseconds

inline void elasticImpactMid(int & velX, int & velY) { velX *= -1; velY *= -1; }
inline void elasticImpact(int & vel) { vel *= -1; } 

Object racketLeft {2, 2, 3};
Object racketRight {7, 2, 3};
Object ball {4, 4, 1};

void setup() {
  //Serial.begin(9600); // debug (OFF!!!)
   
  for (int i = 0; i < ROWS - 1; i++)
  {
    pinMode(rows[i], OUTPUT);
    pinMode(columns[i], OUTPUT);
    digitalWrite(columns[i], HIGH); // all leds are off
  }

  pinMode(playerIn, INPUT);
}

void loop() {
  controlPlayer(racketLeft);
  controlEnemy(racketRight);
  controlBall(ball);
  
  drawObj(racketLeft);
  drawObj(racketRight);
  drawObj(ball);
}

void draw(int x_, int y_)
{
  // there is something weird about this function - 
  // drawing does correctly only if digitalWrite() methods
  // are placed into for loop, another way graphic issues appear.
  
  int x = x_ - 1;
  int y = y_ - 1;

  for (int i = 0; i < COLS; i++)
  {
    if (i == x)
    {
      digitalWrite(columns[i], LOW);   
      digitalWrite(rows[y], HIGH);
      digitalWrite(rows[y], LOW);
      digitalWrite(columns[i], HIGH);
    }
  }
}

void drawObj(Object obj)
{
  for (int i = 0; i < obj.len; i++)
    draw(obj.x, obj.y + i);
}

void controlPlayer(Object & player)
{
  rotat2 = analogRead(playerIn) / 4;

  if (rotat2 - rotat1 > 0)
    player.y++;
  else if (rotat2 - rotat1 < 0)
    player.y--;
  rotat1 = rotat2;

  if (player.y < 1)
    player.y = 1;
  if (player.y > 6)
    player.y = 6;
}

void controlBall(Object & ball)
{
  ballTime2 = millis();

  // ball proceeds every 160's millisecond
  if (ballTime2 - ballTime1 < ballTickSpeed)
    return;
  ballTime1 = ballTime2;  

  // move the ball with its velocity
  ball.x += ballVelX;
  ball.y += ballVelY;

  // ball bounces of rackets and screen's top and bottom
  // bouncing off left racket
  if (ball.x == racketLeft.x && ball.y == racketLeft.y) // TODO: fix this with right physical equations
    if (ballVelY > 0)
      elasticImpactMid(ballVelX, ballVelY);
    else
      elasticImpact(ballVelX);

  if (ball.x == racketLeft.x && ball.y == racketLeft.y + 1)
    elasticImpact(ballVelX);

  if (ball.x == racketLeft.x && ball.y == racketLeft.y + 2)
    if (ballVelY < 0)
      elasticImpactMid(ballVelX, ballVelY);
    else
      elasticImpact(ballVelX);

  // bouncing off right racket
  if (ball.x == racketRight.x && ball.y == racketRight.y)
    if (ballVelY > 0)
      elasticImpactMid(ballVelX, ballVelY);
    else
      elasticImpact(ballVelX);

  if (ball.x == racketRight.x && ball.y == racketRight.y + 1)
    elasticImpact(ballVelX);

  if (ball.x == racketRight.x && ball.y == racketRight.y + 2)
    if (ballVelY < 0)
      elasticImpactMid(ballVelX, ballVelY);
    else
      elasticImpact(ballVelX);
      
  // bouncing off screen's left and right edges  
  if (ball.y <= 1 || ball.y >= 8)
    elasticImpact(ballVelY);

  // reset ball's position and velocity if it leaves screen
  if (ball.x <= 0 || ball.x >= 8)
  {
    ball.x = 4;
    ball.y = 4;
    ballVelX = -1;
    ballVelY = 1;
  }
}

void controlEnemy(Object & enemy)
{
  enemyTime2 = millis();

  // enemy proceeds every 160's millisecond
  if (enemyTime2 - enemyTime1 < enemyTickSpeed)
    return;
  enemyTime1 = enemyTime2; 

  // enemy follows ball 
  if (ballVelY > 0)
    enemy.y++;
  if (ballVelY < 0)
    enemy.y--;

  // enemy can't go away from screen
  if (enemy.y < 1)
    enemy.y = 1;
  if (enemy.y > 6)
    enemy.y = 6;
}
