#include<iostream>
#include<stack>
#include<vector>
#include<stdlib.h>
#include<memory.h>

using namespace std;

#define INVALID_VALUE -1
#define HERO_DIED -2
#define HERO 0
#define SUCCESS 0

struct monster
{
    int blood;
    int attack;
    int cost;
    bool select;
};

struct battle_field
{
    int a_blood;
    int b_blood;
    vector<struct monster> a_side;
    vector<struct monster> b_side;
    vector<struct monster> a_hand;
    vector<struct monster> b_hand;

    int round;
};

struct battle_tree_node
{
    int next_visit;
    int opposite_num;
    struct battle_tree_node *son[8];
    //struct battle_tree_node *right_brother;
};

struct battle_tree_node root;

void next_round(struct battle_field &now_battle_field);

int main()
{
    struct battle_field first_battle_field;
    first_battle_field.round = 0;
	struct monster temp;
	memset(&temp,0,sizeof(struct monster));

	temp.cost=2;
	temp.attack=1;
	temp.blood=2;
	first_battle_field.a_hand.push_back(temp);

	temp.cost=3;
	temp.attack=2;
	temp.blood=3;
	first_battle_field.a_hand.push_back(temp);


	temp.cost=1;
	temp.attack=1;
	temp.blood=1;
	first_battle_field.a_side.push_back(temp);

//0000000000000000000000000000000000000000000
	temp.cost=3;
	temp.attack=4;
	temp.blood=2;
	first_battle_field.b_hand.push_back(temp);

	temp.cost=2;
	temp.attack=1;
	temp.blood=3;
	first_battle_field.b_hand.push_back(temp);

	temp.cost=1;
	temp.attack=1;
	temp.blood=2;
	first_battle_field.b_side.push_back(temp);


    next_round(first_battle_field);

    return 0;
}

#if 0
//利用递归的方法进行攻击树的构建
//now_level是现在的一个层级
//b_side决定可选攻击对象数目
//a_side决定有几个人进行选择
void init_for_battle_tree(struct battle_tree_node *head,int now_level,int a_side_num_level,int b_side_num_per_num)
{
    if(now_level> a_side_num_level) return;
    struct battle_tree_node *pre = NULL;
    struct battle_tree_node *now_node = NULL;

    for(int i =0; i< b_side_num_per_num+1 ; i++)
    {
        now_node = malloc(sizeof(struct battle_tree_node));
        now_node->opposite_num = i;
        now_node->next_visit = 0;
        head->son[i] = now_node;
        init_for_battle_tree(now_node,now_level+1,a_side_num_level,b_side_num_per_num);
    }
}

//判断攻击方式是否合法，比如攻击一个血量已经是负数的小兵
//
int valid_battle(stack<struct battle_tree_node *> &battle_stack,struct battle_field &now_battle_field2)
{
    stack<struct battle_tree_node *> battle_stack_copy;
    battle_stack_copy = battle_stack;
    struct battle_tree_node *passive_side_one ;
    int active_num = 0;
    int passive_num = 0;

    if((now_battle_field.round %2) != 0)
    {
        active_num = now_battle_field2.a_side.size();
    }
    else
    {
        active_num = now_battle_field2.b_side.size();
    }

    for(int i = active_num-1 ;i>=0 ;i-- )
    {
        passive_side_one = battle_stack.top();
        passive_num = passive_side_one.opposite_num;
        if(now_battle_field2.b_side[passive_num].blood<=0)
        {
            return INVALID_VALUE;
        }
        else
        {
        	if(passive_num == HERO)
			{
        			now_battle_field2.a_blood -= now_battle_field2.a_side[i].attack;
					if(now_battle_field2.a_blood <=0)
					{
						return HERO_DIED;
					}
			}
			else
			{
            	now_battle_field2.b_side[passive_num].blood -= now_battle_field2.a_side[i].attack;
            	now_battle_field2.a_side[i].blood -= now_battle_field2.b_side[passive_num].attack;

				//记录攻击记录
			}
        }

        battle_stack.pop();
    }

    vector<struct monster>::iterator it;

    for (it = now_battle_field2.a_side.begin(); it != now_battle_field2.a_side.end(); it++)
	{
		if(it->blood <=0)
			{
				now_battle_field2.a_side.erase(it);
			}
    }

	for (it = now_battle_field2.b_side.begin(); it != now_battle_field2.b_side.end(); it++)
	{
		if(it->blood <=0)
		{
			now_battle_field2.b_side.erase(it);
		}
    }

	return SUCCESS;
}
#endif

void next_round(struct battle_field &now_battle_field)
{
    int n_hand_out_num ;
    int original_hand_num;
    int active_num_level;
    int passive_per_level;
    vector<struct monster> *a_or_b_hand;

    //递归结束
    if(now_battle_field.round>10)
    {
        return ;
    }

    if((now_battle_field.round %2) != 0)
    {
        n_hand_out_num = 1<<now_battle_field.a_hand.size();
        a_or_b_hand = &now_battle_field.a_hand;
        original_hand_num = now_battle_field.a_hand.size();
        cout<<"ori hand num a:"<<original_hand_num<<endl;
        active_num_level = now_battle_field.a_side.size();
        passive_per_level = now_battle_field.b_side.size();
    }
    else
    {
        n_hand_out_num = 1<<now_battle_field.b_hand.size();
        a_or_b_hand = &now_battle_field.b_hand;
        original_hand_num = now_battle_field.b_hand.size();
        cout<<"ori hand num b:"<<original_hand_num<<endl;

        active_num_level = now_battle_field.b_side.size();
        passive_per_level = now_battle_field.a_side.size();
    }

    for(int i = 1 ; i <= n_hand_out_num ; i++ )
    {

		for(int m = 0 ; m < original_hand_num ; m++)
		{
				(*a_or_b_hand)[m].select = 0;
		}
		
        //决定出什么手牌
        for(int j =0 ;j < original_hand_num ;j++)
        {
            if ((i << (31 - j)) >> 31 == -1)
            {
                (*a_or_b_hand)[j].select = 1;
            }
        }

		//判断这样出牌是否合法
        int total_cost = 0;
		for(int k = 0 ; k < original_hand_num ; k++)
		{
				if(1 == (*a_or_b_hand)[k].select)
                {
                    total_cost +=(*a_or_b_hand)[k].cost;
                }
                if(total_cost > now_battle_field.round)
                    continue;
		}



        //待攻击完成后，再出牌
        struct battle_tree_node *head = NULL;

        head = (struct battle_tree_node *)malloc(sizeof(struct battle_tree_node));

        //构造攻击树
        init_for_battle_tree(head,1,active_num_level,passive_per_level);

        //进行攻击合法性检测
        stack<struct battle_tree_node *> battle_stack;
        int head_num = 0;
        int son_num = 0;
        int now_visit = 0;
        struct battle_tree_node *now_node_explore = head;
		int battle_result = 0;

        //遍历battle树算法
        while(head_num != passive_per_level)
        {
            now_visit = now_node_explore->next_visit;

            if(now_visit <passive_per_level)
            {
                now_node_explore->next_visit = now_node_explore->next_visit + 1;
                battle_stack.push(now_node_explore);
                now_node_explore = now_node_explore->son[now_visit];

                if((now_node_explore->son[0] == NULL))
                {
                	battle_result = valid_battle(battle_stack,now_battle_field2)
                    if(SUCCESS == battle_result)
                    {
                        //记录出牌记录
						//完成出牌操作
						for(int i = 0 ; i <= n_hand_out_num ; i++)
						{
							if(1 == a_or_b_hand[i].select)
							{
								now_battle_field.a_side.push_back(now_battle_field.a_hand[i]);
								now_battle_field.b_side.erase(now_battle_field.a_hand.begin() + i);
							}
						}

                        next_round(now_battle_field2);
                    }
					else if(HERO_DIED == battle_result)
					{
						return;
					}
					else
					{
                    	//将每一个round的攻击记录记下来
                    	//然后这个round结束时，全部弹出
                    	//可以用vector，push_back，vector里面可以是
                    	now_node_explore = battle_stack.top();
                    	battle_stack.pop();
					}
                }
            }
            else
            {
                now_node_explore = battle_stack.top();
                battle_stack.pop();
            }

            if(now_node_explore == head)
            {
                head_num++;
            }
        }
    }
}
}


