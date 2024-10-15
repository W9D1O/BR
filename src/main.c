#include <raylib.h>
#include <stdio.h>
#include <math.h>

#define FPS 60
#define alto 600
#define ancho 800
#define LAR ancho*0.1
#define LAL alto*0.1
#define bg 0x181818AA
#define r alto*ancho*0.000039
#define ppy alto*0.8 //Posicion de la paleta sobre el eje Y
#define px ancho*0.2
#define py alto*0.05
#define v FPS*0.1
#define neu -1

const Color colores[] = {LIGHTGRAY, 
GRAY     , 
DARKGRAY , 
YELLOW   , 
GOLD     , 
ORANGE   , 
PINK     , 
RED      , 
MAROON   , 
GREEN    , 
LIME     , 
DARKGREEN, 
SKYBLUE  , 
BLUE     , 
DARKBLUE , 
PURPLE   , 
VIOLET   , 
DARKPURPLE,
BEIGE    , 
BROWN    , 
DARKBROWN, 
WHITE    , 
BLACK    , 
BLANK    , 
MAGENTA  , 
RAYWHITE };

typedef struct cañon {
  Vector2 pos;
  float radio;
  Color colo;

}cañon; 

typedef struct Paleta {
  Rectangle p;
  Color colo;
  float velo; 

}Paleta;

typedef struct Ladri{
  Rectangle l;
  Color colo;
  bool vf;
}Ladri;

void rebote(cañon *cñ,Vector2 *velo, Paleta *p, Vector2 *ant,Ladri lr[])
{
for(int i = 0; i < 16; ++i){
    if(lr[i].vf && cñ->pos.x >= lr[i].l.x && cñ->pos.x <= lr[i].l.x + lr[i].l.width && cñ->pos.y + cñ->radio >= lr[i].l.y && cñ->pos.y + cñ->radio <= lr[i].l.y + lr[i].l.width){
    velo->y = velo->y * neu;
      lr[i].vf = false;
    }
}
  if (cñ->pos.x - cñ->radio <= 0 || cñ->pos.x + cñ->radio >= ancho) {
   velo->x = velo->x * neu;
  }
  if (cñ->pos.y - cñ->radio <= 0 || cñ->pos.y + cñ->radio >= alto) {
   velo->y = velo->y * neu;
  }
  if(cñ->pos.y + cñ->radio >= p->p.y  && cñ->pos.y <= p->p.y + py + cñ->radio && cñ->pos.x + cñ->radio >= p->p.x && cñ->pos.x - cñ->radio<= p->p.x + px ){
    if(ant->y + cñ->radio >= p->p.y && (ant->x <= p->p.x || ant->x >= p->p.x + px)){
      if(IsKeyDown(KEY_A) || IsKeyDown(KEY_D)){
        velo->y = velo->y*1.5;
        velo->x = velo->x*1.5;
      }
      velo->y = velo->y * neu;
      velo->x = velo->x * neu;
    }
      else
    velo->y = velo->y * neu;
  }
  ant->y = cñ->pos.y;
  ant->x = cñ->pos.x;
  cñ->pos.x += velo->x;
  cñ->pos.y += velo->y;
}

float mov(float x,float velo)
{
  if(IsKeyDown(KEY_A) && x > 0){
    x += velo*neu;
    if(x < 0) x = 0;
  }
  if(IsKeyDown(KEY_D) && x + px <= ancho){
    x +=velo;
  }
  return x;
}

void render(cañon *cñ,Vector2 *velo, Paleta *p,Vector2 *ant,Ladri lr[],bool *juega)
{
  if(IsKeyPressed(KEY_SPACE)){
    *juega = true;
  }
  if(*juega){
    rebote(cñ,velo,p,ant,lr);
    p->p.x = mov(p->p.x,p->velo);
  }
  for(int i = 0; i < 16; ++i){
    if (!lr[i].vf) lr[i].colo = GetColor(bg);
    DrawRectangle(lr[i].l.x,lr[i].l.y,lr[i].l.width,lr[i].l.height,lr[i].colo);
  } 
  DrawRectangle(p->p.x,p->p.y,p->p.width,p->p.height,p->colo);
  DrawCircle(cñ->pos.x,cñ->pos.y,cñ->radio,cñ->colo);
}
void norm(Vector2 *velo, cañon cñ)
{

  if (cñ.pos.x - cñ.radio*2 <= 0 || cñ.pos.x + cñ.radio*2 >= ancho || cñ.pos.y - cñ.radio*2 <= 0 || cñ.pos.y + cñ.radio*2 >= alto){
    if(abs(velo->x) && abs(velo->y) > v){
      if(velo->x > 0){
        velo->x = v;
      }
      else{
        velo->x = -v;
      }
      if(velo->y > 0){
        velo->y = v;
      }
      else{
        velo->y = -v;
      }
    }
  }
}

void init_l(Ladri lr[])
{ float y = 40.f;
  int x = 0;
  for(int i = 0; i < 16; ++i){
   if(i == 8){
      y =y + LAL;
      x = 0;
    } 
   lr[i].l.width = LAR*1.1;
   lr[i].l.height = LAL;
   lr[i].l.x = (LAR*1.1*x)+LAR*1.1/2;
   lr[i].l.y = y;
    lr[i].colo = colores[i];
    lr[i].vf = true;
    x += 1;
  }
}
int main()
{
    bool juega = false;
    cañon cñ;
    Paleta pl;
    Ladri lr[16];
    init_l(lr);
    Vector2 velo = {
    .x = v,
    .y = v,
  };
    cñ.pos.x = ancho/2;
    cñ.pos.y = ppy;
    cñ.radio = r; 
    cñ.colo = SKYBLUE;
    pl.p.y = ppy + cñ.radio;
    pl.p.x = (ancho / 2 - px / 2);
    pl.p.width = px;
    pl.p.height = py;
    pl.velo = v;
    pl.colo = RED;
    Vector2 ant = {
    .x = 0,
    .y = 0,
  };
    SetTargetFPS(FPS);
    InitWindow(ancho,alto,"Breaker");
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GetColor(bg));
        norm(&velo,cñ);
        render(&cñ,&velo,&pl,&ant,lr,&juega);
        EndDrawing();
    }
  CloseWindow();
    
    return 0;
}
