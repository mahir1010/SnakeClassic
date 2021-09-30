#include "SnakeNetwork.h"
#include "config.h"
Command next_command(TCPsocket client,Food *food,RenderingLines *head)
{   
    Command cmd=STOP;
    int length=0;
    RenderingLines *temp=head;
    while(temp!=NULL){
        length++;
        temp=temp->next;
    }
    int size=sizeof(int)*2+length*sizeof(Attributes);
    int data[size/4];
    data[0]=food->x1+2;
    data[1]=food->y1+2;
    int i=2;
    temp=head;
    while(temp!=NULL){
        data[i]=temp->attr->x1;
        data[i+1]=temp->attr->y1;
        data[i+2]=temp->attr->x2;
        data[i+3]=temp->attr->y2;
        data[i+4]=temp->attr->x1_increment;
        data[i+5]=temp->attr->y1_increment;
        data[i+6]=temp->attr->x2_increment;
        data[i+7]=temp->attr->y2_increment;
        i+=8;
        temp=temp->next;
    }
    SDLNet_TCP_Send(client,&size,sizeof(size));
    int r=SDLNet_TCP_Send(client,&data,sizeof(data));
    int start=SDL_GetTicks();
    while(cmd==0x05){
        SDLNet_TCP_Recv(client,&cmd,1);
        if (SDL_GetTicks()-start>1000){
        break;
        }
    }
    if (SDL_GetTicks()-start>1000){
        cmd=NOOP;
    }
    return cmd;
}

void connect_controller(TCPsocket *client){
    IPaddress ip;
    TCPsocket tcpsock;

    if(SDLNet_ResolveHost(&ip,NULL,4668)==-1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }

    tcpsock=SDLNet_TCP_Open(&ip);
    if(!tcpsock) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(2);
    }
    while(*client==NULL){
        *client=SDLNet_TCP_Accept(tcpsock);
    }
    int dims[2]={RES_ROW,RES_COL};
    SDLNet_TCP_Send(*client,dims,sizeof(dims));
}
