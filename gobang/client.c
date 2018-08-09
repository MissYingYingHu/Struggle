#include "client.h"

#define ROW 15
#define COL 15

int arr[ROW][COL];

// ��ʼ������
void InitArr()
{
    int i = 0;
    for (; i < ROW; i++)
    {
        int j = 0;
        for (; j < COL; ++j)
        {
            arr[i][j] = ' ';
        }
    }
}
// ��Ϸ����
void Chessboard()
{
    int w = 0;
    for (; w < COL; ++w)
    {
        printf("%4d", w);
    }
    printf("\n");

    int i = 0;
    for (; i < ROW; ++i)
    {
        printf("%2d",i);

        int j = 0;
        for (; j < COL; ++j)
        {
            printf(" %c |", arr[i][j]);
        }
        printf("\n");

        printf("  ");
        int q = 0;
        for (;q < COL; ++q)
        {
            printf("---|");
        }
        printf("\n");
    }
}

// ��Ϸ�˵�
int menu()
{
    while (1)
    {
        printf("==========================\n");
        printf("  1 �˻����� 2 ��������   \n");
        printf("          0 �˳�          \n");
        printf("==========================\n");
        char buf[512];
        ssize_t rd = read(0, &buf, sizeof(buf)-1);
        if (rd < 0)
        {
            perror("��ȡ����������ѡ��\n");
            continue;
        }
        buf[rd-1] = '\0';

        fflush(stdin);
        if (strlen(buf) != 1)
        {
            printf("�������\n");
            continue;
        }
        if (buf[0] == '2')
        {
            return 2;
        }
        else if (buf[0] == '1')
        {
            return 1;
        }
        else if (buf[0] == '0')
        {
            printf("��Ϸ����\n");
            exit(0);
        }
        else
        {
            printf("�������\n");
        }
    }
    return 0;
}

// ����Ϸ��ж�
Coordinate InputToDecide()
{
    Coordinate coor;
    printf("����������~ Usage��x y\n");
    while (1)
    {
        scanf("%d%d", &(coor.x), &(coor.y));
        if (coor.x < 0 || coor.x >= ROW || coor.y < 0 || coor.y >= COL)
        {
            printf("������������,����������~\n");
            continue;
        }
        if (arr[coor.x][coor.y] == ' ')
        {
            break;
        }
        printf("����������Ѿ�����,����������~\n");
    }
    return coor;
}

int DecideWinLoss(Coordinate coor, char c)
{
    Coordinate _coor;
    _coor.x = coor.x;
    _coor.y = coor.y;
    // ���·���
    int count = 0;
    while (coor.x >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.x;
    }
    ++coor.x;
    while (coor.x < ROW && arr[coor.x][coor.y] == c)
    {
        ++coor.x;
        ++count;
    }
    if (count >= 5)
    {
        // 0����ɹ�
        return 0;
    }

    // ���ҷ���
    count = 0;
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.x >= 0 && coor.y < COL && arr[coor.x][coor.y] == c)
    {
        --coor.x;
        ++coor.y;
    }
    ++coor.x;
    --coor.y;
    while (coor.x < ROW && coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
       ++coor.x;
       --coor.y;
       ++count;
       /* printf("1\n"); */
    }
    if (count >= 5)
    {

        /* printf("��you\n"); */
        return 0;
    }

    // ����
    count = 0;
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.y < COL && arr[coor.x][coor.y] == c)
    {
        ++coor.y;
    }
    --coor.y;
    while (coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.y;
        ++count;
    }
    if (count >= 5)
    {
        /* printf("you\n"); */
        return 0;
    }

    // ����
    count = 0;
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.y < COL && coor.x < ROW && arr[coor.x][coor.y] == c)
    {
        ++coor.x;
        ++coor.y;
    }
    --coor.x;
    --coor.y;
    while (coor.x >= 0 && coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.x;
        --coor.y;
        ++count;
    }
    if (count >= 5)
    {
        return 0;
    }
    return -1;
}

// �Ƚ������ӵĿͻ���
void FirstTalk(int sock)
{
    while (1)
    {
        // �ж������Ƿ�Ϸ�
        Coordinate coor = InputToDecide();
        
        // ������
        arr[coor.x][coor.y] = '$';
        Chessboard();

        int decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        printf("coor %d %d", coor.x, coor.y);
        coor.x = htonl(coor.x);
        coor.y = htonl(coor.y);
        write(sock, &coor, sizeof(coor));

        if (decide == 0)
        {
            printf("��ϲ��~ ʤ����~\n");
            close(sock);
            return;
        }

        printf("�Է�������......\n");
        ssize_t rd = read(sock, &coor, sizeof(coor));
        if (rd < 0)
        {
            perror("read");
            return;
        }

        if (rd == 0)
        {
            printf("read done!");
            return;
        }
        coor.x = ntohl(coor.x);
        coor.y = ntohl(coor.y);

        arr[coor.x][coor.y] = '@';
        Chessboard();
        printf("����: x = %d, y = %d", coor.x, coor.y);

        // ��������ж���Ӯ
        decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        if (decide == 0)
        {
            printf("���ź�~ ������~\n");
            close(sock);
            return;
        }
    }
}

// �������ӵ�
void SecondTalk(int sock)
{
    Coordinate coor;
    while (1)
    {
        printf("�Է�������......\n");
        ssize_t rd = read(sock, &coor, sizeof(coor));
        if (rd < 0)
        {
            perror("read");
            return;
        }

        if (rd == 0)
        {
            printf("read done!");
            return;
        }
        coor.x = ntohl(coor.x);
        coor.y = ntohl(coor.y);
       
        arr[coor.x][coor.y] = '$';
        Chessboard();
        printf("�Է�: x = %d, y = %d", coor.x, coor.y);
        int decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        if (decide == 0)
        {
            printf("���ź�~ ������~\n");
            close(sock);
            return;
        }

        coor = InputToDecide();
        arr[coor.x][coor.y] = '@';
        Chessboard();

        decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        coor.x = htonl(coor.x);
        coor.y = htonl(coor.y);
        write(sock, &coor, sizeof(coor));

        if (decide == 0)
        {
            printf("��ϲ��~ ʤ����~\n");
            close(sock);
            return;
        }
    }
}

// ��������
int Connect(char* ip, char* port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return 2;
    }

    // ��������
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(atoi(port));
    int n = connect(sock, (struct sockaddr*)&server, sizeof(server));
    if(n < 0)
    {
        perror("connect");
        return 3;
    }
    return sock;
}

//////////////////////////
//ע��Ϊ����ע�ͣ��������·����ͬ
//////////////////////////
//��������λ������
// �ж����·����Ƿ��������
Coordinate UpDown(Coordinate coor)
{
    int x = coor.x;
    int y = coor.y;
    while (x >= 0 && arr[x][y] == '$')
    {
        --x;
    }
    if (x >= 0 && arr[x][y] == ' ')
    {
        coor.x = x;
        coor.y = y;
        return coor;
    }
    if (x < 0 || arr[x][y] == '@')
    {
        x = coor.x;
        y = coor.y;
        while (x < ROW && arr[x][y] == '$')
        {
            ++x;
        }
        coor.x = x;
        coor.y = y;
        return coor;
    }
    return coor;
}

Coordinate UpRight(Coordinate coor)
{
    int x = coor.x;
    int y = coor.y;
    while (x >= 0 && y < COL && arr[x][y] == '$')
    {
        --x;
        ++y;
    }
    if (x >= 0 && y < COL && arr[x][y] == ' ')
    {
       coor.x = x;
       coor.y = y;
       return coor;
    }
    if (x < 0 || y == COL || arr[x][y] == '@')
    {
        x = coor.x;
        y = coor.y;
        while (x < ROW && y >= 0 && arr[x][y] == '$')
        {
            ++x;
            --y;
        }
        coor.x = x;
        coor.y = y;
    }
    // ���������ԭ����λ��
    return coor;
}

Coordinate LeftRight(Coordinate coor)
{
    int x = coor.x;
    int y = coor.y;
    while (y >= 0 && arr[x][y] == '$')
    {
        --y;
    }
    if (y >= 0 && arr[x][y] == ' ')
    {
        coor.x = x;
        coor.y = y;
        return coor;
    }
    if (y < 0 || arr[x][y] == '@')
    {
        x = coor.x;
        y = coor.y;
        while (y < COL && arr[x][y] == '$')
        {
            ++y;
        }
        coor.x = x;
        coor.y = y;
        return coor;
    }
    return coor;
}

Coordinate RightDown(Coordinate coor)
{
    int x = coor.x;
    int y = coor.y;
    // ��ȥ�ж����Ͻ����ж�
    while (x >= 0 && y >= 0 && arr[x][y] == '$')
    {
        --x;
        --y;
    }
    // �����ϵ����һ���ǲ��ǡ� ��
    if (x >= 0 && y >= 0 && arr[x][y] == ' ')
    {
        coor.x = x;
        coor.y = y;
        return coor;
    }
    // �����������λ���Լ��Ѿ����ˣ����ߵ��˱߽�
    if (x < 0 || y < 0 || arr[x][y] == '@')
    {
        // �ص����������λ��
        x = coor.x;
        y = coor.y;
        // �������·�����ж�
        while (x < ROW && y < COL && arr[x][y] == '$')
        {
            ++x;
            ++y;
        }
        // ��Ϊ���ʼѰ���·��ʱ���Ѿ��ж������˶����������������������һ������һ��λ��
        coor.x = x;
        coor.y = y;
        return coor;
    }
    // ���൱���쳣
    // ���û�н������е��ж���䷵��ԭ������ߣ��ں�����ڽ����жϡ�
    return coor;
}
// ��һ����������
// ͨ����һȦ���Ƚϳ�����·���ϵ��������
// c �� $
Coordinate AroundHave(Coordinate coor, char c)
{
    Coordinate _coor;
    _coor.x = coor.x;
    _coor.y = coor.y;
    int count[4] = {0};
    // ���·���
    // ���ߵ�����
    while (coor.x >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.x;
    }
    // �ж��ǲ������ϵ���һ���ǶԷ����ӣ�Ϊ����������ų�һ�����˶����������
    int fg0 = 0;
    if (coor.x < 0 || arr[coor.x][coor.y] == '@')
    {
        fg0 = 1;
    }
    // ��������λ��
    ++coor.x;
    // ������������·�еĶԷ�������
    while (coor.x < ROW && arr[coor.x][coor.y] == c)
    {
        ++coor.x;
        ++count[0];// ��¼��������
    }
    // ������¶Է��������Ѿ������Ƕ�������ʱ��Ͳ��ù����������
    if ((coor.x ==  ROW || arr[coor.x][coor.y] == '@') && fg0 == 1) //�ж�����Ϊ�˽�����·���ϵ������Ѿ�������
    {
        count[0] = 0;
    }

    /////// ���������߼�д�������ϡ���������
    // ���ҷ���
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.x >= 0 && coor.y < COL && arr[coor.x][coor.y] == c)
    {
        --coor.x;
        ++coor.y;
    }
    int fg1 = 0;
    if (coor.x == 0 || coor.y == COL || arr[coor.x][coor.y] == '@')
    {
        fg1 = 1;
    }
    ++coor.x;
    --coor.y;
    while (coor.x < ROW && coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
       ++coor.x;
       --coor.y;
       ++count[1];
    }
    if ((coor.x == ROW || coor.y == 0 || arr[coor.x][coor.y] == '@') && fg1 == 1)
    {
        count[1] = 0;
    }

    // ����
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.y < COL && arr[coor.x][coor.y] == c)
    {
        ++coor.y;
    }
    int fg2 = 0;
    if (coor.y == COL || arr[coor.x][coor.y] == '@')
    {
        fg2 = 1;
    }
    --coor.y;
    while (coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.y;
        ++count[2];
    }
    if ((coor.y == 0 || arr[coor.x][coor.y] == '@') && fg2 == 1)
    {
        count[2] = 0;
    }

    // ����
    coor.x = _coor.x;
    coor.y = _coor.y;
    while (coor.y < COL && coor.x < ROW && arr[coor.x][coor.y] == c)
    {
        ++coor.x;
        ++coor.y;
    }
    int fg3 = 0;
    if (coor.y == COL || coor.x == ROW || arr[coor.x][coor.y] == '@')
    {
        fg3 = 1;
    }
    --coor.x;
    --coor.y;
    while (coor.x >= 0 && coor.y >= 0 && arr[coor.x][coor.y] == c)
    {
        --coor.x;
        --coor.y;
        ++count[3];
    }
    if ((coor.x == 0 || coor.y == 0 || arr[coor.x][coor.y] == '@') && fg3 == 1)
    {
        count[3] = 0;
    }

    // �����ж��ĸ�·���ϵ����Ӷ�
    int count_num = 0;
    int i = 0;
    for (; i < 4; ++i)
    {
        int j = 0;
        for (; j < 4; ++j) 
        {
            if (count[i] < count[j])
            {
                break;
            }
        }
        if (j == 4)
        {
            /* printf("�����Ƚ����Ǹ�·�Ͻ�������%d", i); */
            count_num = i;
            break;
        }
    }
    
    // ���з�֧�ж�, ��Ҫ������·�Ͻ�������
    if (count_num == 0)
    {
        // ����·
        Coordinate ud = UpDown(_coor);// ���������·�ϵ����Ǳ�����;
        if (ud.x == _coor.x && ud.y == _coor.y)
        {
            perror("UpDown error\n");
            return coor;
        }
        // ���Ǵ����µ�����Ϊ��0�е�ʱ������Ҫ�����·�
        // ��ֹ����Խ��
        if (ud.x < 0)
        {
            ++ud.x;
            while (arr[ud.x][ud.y] == '$')
            {
                ++ud.x;
            }
            return ud;
        }
        return ud;

    }
    else if (count_num == 1)
    {
        // ���Һ���������ж�
        Coordinate ur = UpRight(_coor);
        if (ur.x == _coor.x && ur.y == _coor.y)
        {
            perror("UpRight error\n");
            return coor;
        }
        return ur;
    }
    else if (count_num == 2)
    {
        // ���� �ж�
        Coordinate lr = LeftRight(_coor);
        if (lr.x == _coor.x && lr.y == _coor.y)
        {
            perror("LeftRight error\n");
            return coor;
        }
        return lr;
    }
    else
    {
        // ���º������ж�
        Coordinate rd = RightDown(_coor);
        if (rd.x == _coor.x && rd.y == _coor.y)
        {
            perror("RightDown error\n");
            return coor;
        }
        return rd;
    }

    // �������ɳ����Զ���һ�����֣��ط����ж����Զ�������
    // �򷵻ش�����Ϣ
    Coordinate tmp;
    tmp.x = 100;
    tmp.y = 100;
    return tmp;
}


// ����Ԥ���ӿڣ�Ϊ������������ӵĳ��򣬽��и����Ѷȵļ�������
Coordinate ProductPos(Coordinate coor)
{
   return AroundHave(coor, '$');
}

// �˻���ս
void PeopleFightMachine()
{
    // ��ʼ������
    InitArr();
    while (1)
    {
        Chessboard();
        printf("������~\n");
        // �жϺϷ�����
        Coordinate coor = InputToDecide();
        
        // ������
        arr[coor.x][coor.y] = '$';
        Chessboard();
        // �ж���Ӯ
        int decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        if (decide == 0)
        {
            printf("��ϲ��~ ���ܶ���~\n");
            return;
        }

        // ������
        // ���Է�����Ҫ��λ��
        coor = ProductPos(coor);
        //
        if (coor.x == 100)
        {
            perror("����\n");
            return;
        }
        
        // ������
        arr[coor.x][coor.y] = '@';
        Chessboard();

        decide = DecideWinLoss(coor, arr[coor.x][coor.y]);
        if (decide == 0)
        {
            printf("���ź�~ ������~\n");
            return;
        }
    }
}

// Ҫ����./client IP port �����в���
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        perror("usage: ./client [ip] [port]");
        return 1;
    }

    ///////////////////////////////////
    // �����˻���ս
    ///////////////////////////////////
    while (1)
    {
        // �˵�
        int me = menu(); // ��Ϸ��ʼ
        if (me == 1)
        {
            // �˻���ս
            PeopleFightMachine();
        }
        else if (me == 2) // ѡ��2 ˵��Ҫ���������ս
        {
            // ����ȥ˵�����������ս
            break;
        }
    }

    ///////////////////////////////////
    // �����ս,ģ��
    ///////////////////////////////////

    // ��������
    int sock = Connect(argv[1], argv[2]);

    // �������շ��������͵�ID �����ж��Ǻڷ����ǰ׷�
    int i = 0;
    read(sock, &i, sizeof(int));
    /* printf("%d", i); */
    if (i == 0)
    {
        // �ڷ�����
        InitArr();
        Chessboard();
        FirstTalk(sock);
    }
    else if (i == 1)
    {
        // �׷�����
        InitArr();
        Chessboard();
        SecondTalk(sock);
    }
    else
    {
        perror("�Ⱥ�˳�����");
        return -1;
    }
    return 0;
}
