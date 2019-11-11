#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <zconf.h>


typedef struct tetromino {
    int tetro[4][4];
    int posx;
    int posy;
} tetromino;

typedef struct estado {
    int mapa[23][10];
    tetromino atual;
    tetromino prox;
    int score;
    int nivel;
    int lines;
    int tetris;
} estado;

int notover (estado e) {
    for (int i = 0; i < 10; i++) {
        if (e.mapa[3][i] == 1)
            return 0;
    }
    return 1;
}

tetromino proximotetro () {
    int t,i,j;
    tetromino tetromino;
    srand(time(NULL));
    t= (rand() % 7) + 1;
    switch (t) {
        case (1): // tetronimo forma em T
        {
            for (i=0;i<4;i++) {
                for (j=0;j<4;j++) {
                    if ((i==2 && j<3) || (i==1 && j==1))
                        tetromino.tetro[i][j]=1;
                    else tetromino.tetro[i][j]=0;
                }
            }
        }
            break;
        case (2): // tetronimo forma S
        {
            for (i=0;i<4;i++) {
                for (j=0;j<4;j++) {
                    if ((i==1 && j>0 && j<3) || (i==2 && j<2))
                        tetromino.tetro[i][j]=1;
                    else tetromino.tetro[i][j]=0;
                }
            }
        }
            break;
        case (3) : //tetronimo forma Z
        {
            for (i=0;i<4;i++) {
                for (j=0;j<4;j++) {
                    if ((i==2 && j>0 && j<3) || (i==1 && j<2))
                        tetromino.tetro[i][j]=1;
                    else tetromino.tetro[i][j]=0;
                }
            }
        }
            break;
        case (4): // tetronimo forma L
        {
            for (i=0;i<4;i++) {
                for (j=0;j<4;j++) {
                    if ((i==1 && j==2) || (i==2 && j<3))
                        tetromino.tetro[i][j]=1;
                    else tetromino.tetro[i][j]=0;
                }
            }
        }
            break;
        case (5): //tetromino forma ⅃
        {
            for (i=0;i<4;i++) {
                for (j=0;j<4;j++) {
                    if ((i==2 && j<3) || (i==1 && j==0))
                        tetromino.tetro[i][j]=1;
                    else tetromino.tetro[i][j]=0;
                }
            }
        }
            break;
        case (6): //tetromino forma o
        {
            for (i=0;i<4;i++) {
                for (j=0;j<4;j++) {
                    if ((i==1 && j>0 && j<3) || (i==2 && j<3 && j>0))
                        tetromino.tetro[i][j]=1;
                    else tetromino.tetro[i][j]=0;
                }
            }
        }
            break;
        case (7): //tetromino forma |
        {
            for (i=0;i<4;i++) {
                for (j=0;j<4;j++) {
                    if (i==2)
                        tetromino.tetro[j][i]=1;
                    else tetromino.tetro[j][i]=0;
                }
            }
        }
    }
    tetromino.posy=0;
    tetromino.posx=4;
    return tetromino;
}


estado limpalinhas (estado e) {
    int linhas=0,i,j=1,x,y;
    for (i=0;i<24;i++) {
        if (e.mapa[i][0]==1) {
            while (e.mapa[i][j] == 1 && j<10) j++;
            if (j==10) {
                linhas++;
                for (y=i;y>0;y--) {
                    for (x = 0; x < 10; x++)
                        e.mapa[y][x] = e.mapa[y - 1][x];
                }
                for (x=0;x<10;x++) e.mapa[0][x]=0;
            }
            j=1;
        }

    }
    e.lines+=linhas;
    switch (linhas) {
        case (1) :
            linhas = 40;
            break;
        case (2) :
            linhas = 100;
            break;
        case (3) :
            linhas = 300;
            break;
        case (4) : {
            linhas = 1200;
            e.tetris++;
        }
            break;
        default:
            break;
    }
    e.score+=linhas*(e.nivel+1);
    return e;
}

estado new () {
    estado novo;
    for (int i=0;i<23;i++) {
        for (int j=0;j<10;j++) {
            novo.mapa[i][j]=0;
        }
    }
    novo.atual=proximotetro();
    novo.prox=proximotetro();
    novo.score=0;
    novo.nivel=0;
    novo.lines=0;
    novo.tetris=0;
    return novo;
}

int pode (estado e) {
    int ytetro,xtetro,i,j;
    tetromino t=e.atual;
    ytetro=e.atual.posy;
    xtetro=e.atual.posx;
    for (i=0;i<4;i++) {
        for (j=0;j<4;j++) {
            if (t.tetro[i][j])
                if (e.mapa[ytetro+i][xtetro+j-1] || ytetro+i>22||xtetro+j>10 || ytetro+i<0 || xtetro+j<1)
                    return 0;
        }
    }
    return 1;
}


estado acrescenta (estado e) {
    int i,j,y,x;
    y=e.atual.posy-1;
    x=e.atual.posx;
    for (i=0;i<4;i++) {
        for (j=0;j<4;j++) {
            if (e.atual.tetro[i][j])
                e.mapa[y+i][x+j-1]=1;
        }
    }
    e.atual=e.prox;
    e.prox=proximotetro();
    return e;
}

estado TurnR(estado e) {
    int i,k, keeper[4][4];
    for (k = 3; k > -1; k--) {
        for (i = 0; i < 4; i++) {
            keeper[i][k] = e.atual.tetro[3-k][i];
        }
    }
    for (k=0;k<4;k++) {
        for (i=0;i<4;i++) {
            e.atual.tetro[i][k]=keeper[i][k];
        }
    }
    return e;
}

estado TurnL(estado e) {
    int i,k, keeper[4][4];
    for (k = 3; k > -1; k--) {
        for (i = 0; i < 4; i++) {
            keeper[i][k] = e.atual.tetro[k][3-i];
        }
    }
    for (k=0;k<4;k++) {
        for (i=0;i<4;i++) {
            e.atual.tetro[i][k]=keeper[i][k];
        }
    }
    return e;
}


estado jogada (estado atual,int c) {
    if (c==ERR) c='s';
    switch (c) {
        case ('d') : {
            atual.atual.posx++;
            if (pode(atual)==0) {
                atual.atual.posx--;
                atual = jogada(atual, 's');
            }
        }
        break;
        case ('a') : {
            atual.atual.posx--;
            if (pode(atual)==0) {
                atual.atual.posx++;
                atual=jogada(atual,'s');
            }
        }
        break;
        case ('s') : {
            atual.atual.posy++;
             if (pode(atual)==0)
                atual=acrescenta(atual);
             }
        break;
        case ('w'): {
            atual.atual.posy--;

            if (pode(atual)==0) atual.atual.posy++;
        }
            break;
        case ('y'): {
            atual = TurnR(atual);
            if (pode(atual) == 0) atual = TurnL(atual);
        }
            break;
        case ('t'): {
            atual=TurnL(atual);
            if (pode(atual)==0) atual=TurnR(atual);
        }
            break;
        case (' '): {
            while (pode(atual)) atual.atual.posy++;
            atual.atual.posy--;
            acrescenta(atual);
        }
        break;
        default:
            break;
    }
    return atual;
}



int desenha (int l,int c) {
    estado e;
    FILE *scores;
    int hatual,watual,k,ch,htemp,wtemp,i,j,topscore;
    char nome[13],nome2[13];
    e=new();
    initscr();
    WINDOW *win=newwin(24, 24, l, c);
    WINDOW *score=newwin(24,28,l+1,c+26),*proxpeca=newwin(6,12,l+15,c+32);
    WINDOW *highscores,*novoscore;
    halfdelay(1);
    curs_set(0);
    keypad(win,TRUE);
    noecho();
    while (notover(e)) {
        wclear(proxpeca);
        wclear(win);
        e=limpalinhas(e);
        box(score,0,0);
        i=(e.lines==0) ? 0:e.tetris*4*100/(e.lines);
        mvwprintw(score,1,2,"SCORE:");
        mvwprintw(score,2,2,"%d",e.score);
        mvwprintw(score,4,2,"Lines cleared:");
        mvwprintw(score,5,2,"%d",e.lines);
        mvwprintw(score,7,2,"Number of tetris:");
        mvwprintw(score,8,2,"%d",e.tetris);
        mvwprintw(score,10,2,"TETRIS percentage: %d %",i);
        box(proxpeca,0,0);
        for (htemp=0;htemp<4;htemp++) {
            for (wtemp=0;wtemp<4;wtemp++) {
                if (e.prox.tetro[htemp][wtemp] == 1) {
                    mvwaddch(proxpeca, 1 + htemp, 2 + 2 *wtemp + 1, ' ' | A_REVERSE);
                    mvwaddch(proxpeca, 1 + htemp, 2 + 2 *wtemp, ' ' | A_REVERSE);
                }
            }
        }
        mvwprintw(proxpeca,0,1,"Next Tetro");
        wrefresh(score);
        wrefresh(proxpeca);
        ch=getch();
        for (j=0;j<24;j++) {
            mvwaddch(win,2,j,' '|A_REVERSE);
            mvwaddch(win,23,j,' '|A_REVERSE);
            mvwaddch(win,j+2,0,' '|A_REVERSE);
            mvwaddch(win,j+2,1,' '|A_REVERSE);
            mvwaddch(win,j+2,23,' '|A_REVERSE);
            mvwaddch(win,j+2,22,' '|A_REVERSE);
        }
        /*switch (ch) {
            case (KEY_RIGHT) :ch=1;
                break;
            case (KEY_LEFT) :ch=2;
                break;
            case (KEY_DOWN) : ch=3;
                break;
            case (KEY_UP) : ch=4;
                break;
            default:
                break;
        }
         */
        e=jogada(e,ch);
        i=e.atual.posy;
        j=e.atual.posx;
        for (hatual = 0; hatual < 24; hatual++) {
            for (watual = 0; watual < 10; watual++) {
                if (e.mapa[hatual][watual] == 1) {
                    k = 2 * (watual + 1);
                    mvwaddch(win, hatual, 1 + k, ' ' | A_REVERSE);
                    mvwaddch(win, hatual, k, ' ' | A_REVERSE);
                }
                if (i == hatual) {
                    for (htemp = 0; htemp < 4; htemp++) {
                        for (wtemp = 0; wtemp < 4; wtemp++) {
                            if (e.atual.tetro[htemp][wtemp]) {
                                if (j < 0) {
                                    mvwaddch(win, hatual + htemp, 2 * (wtemp - 1), ' ' | A_REVERSE);
                                    mvwaddch(win, hatual + htemp, 2 * (wtemp - 1) + 1, ' ' | A_REVERSE);
                                } else if (j == watual) {
                                    mvwaddch(win, hatual + htemp, 2 * (wtemp + watual), ' ' | A_REVERSE);
                                    mvwaddch(win, hatual + htemp, 2 * (wtemp + watual) + 1, ' ' | A_REVERSE);
                                }
                            }
                        }
                    }
                }
            }
        }
        wrefresh(win);
    }
    wclear(win);
    wrefresh(win);
    delwin(win);
    refresh();
    win=newwin(5,26,l,c);
    highscores=newwin(14,26,l+6,c);
    box(highscores,0,0);
    scores=fopen("scores","r");
    for (i=2;i<12;i+=2) {
        fgets(nome,12,scores);
        fscanf(scores,"%d\n",&topscore);
        if (e.score>topscore) {
            novoscore=newwin(8,26,l+5,c);
            box(novoscore,0,0);
            mvwprintw(novoscore,1,2,"New Highscore!!!");
            mvwprintw(novoscore,4,2,"Insert penis daddy");
            halfdelay(100);
            getch();
            for (j=0;j<10;j++) {
                ch = getch();
                if (ch=='\n') break;
                nome2[j]=ch;
                mvwaddch(novoscore,6,2+j,ch);
                wrefresh(novoscore);
            }
            wrefresh(novoscore);
            for (;j<10;j++) nome2[j]='.';
            nome2[10]='\0';
            mvwprintw(highscores,i,4,"%d.%s%d",i/2,nome2,e.score);
            i+=2;
            wclear(novoscore);
            delwin(novoscore);
            refresh();
        }
        if (i<12)
        mvwprintw(highscores, i, 4,"%d.%s%d",i/2,nome,topscore);
    }
    fclose(scores);
    wrefresh(highscores);
    box(win,0,0);
    mvwprintw(win,1,2,"Obrigado por jogar");
    mvwprintw(win,3,2,"Press enter");
    wrefresh(win);
    mvprintw(l+5,c+8,"HIGHSCORES");
    while (getch()!='\n');
    delwin(score);
    endwin();
    return 0;
}




int main() {
    int c = 0, l = 0, i, k;
    char lista[5][20] = {"Primeira", "Segunda", "Terceira", "Quarta", "Quinta"};
    int ch;
    initscr();
    getmaxyx(stdscr, l, c);
    l = l / 2 - 9;
    c = c / 2 - 9;
    WINDOW *win = newwin(7, 18, l, c);
    refresh();
    box (win, 0, 0);
    for (i = 0; i < 5; i++) {
        if (i == 0)
            wattron(win, A_STANDOUT);
        else
            wattroff(win, A_STANDOUT);
        mvwprintw(win, i + 1, 2, "%s", lista[i]);
    }
    wrefresh(win);
    i = 0;
    keypad(win, TRUE);
    curs_set(0);
    while ((ch = wgetch(win)) != '\n') {
        switch (ch) {
            case (KEY_DOWN) : {
                i++;
                i = (i < 5) ? i : 0;
            }
                break;
            case (KEY_UP) : {
                i--;
                i = (i < 0) ? 4 : i;
            }
            default:
                break;
        }
        for (k = 0; k < 5; k++) {
            if (k == i)
                wattron(win, A_STANDOUT);
            else
                wattroff(win, A_STANDOUT);
            mvwprintw(win, k + 1, 2, "%s", lista[k]);
        }
        wrefresh(win);
    }
    delwin(win);
    desenha(l, c);
    endwin();
    return 0;
}
