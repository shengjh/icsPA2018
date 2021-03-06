#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char *exp){
	WP *fwp = free_;
	if(free_ == NULL) assert(0);
	else{
		if(free_->next == NULL)
			free_ = NULL;
		else{
			fwp = free_->next;
			free_->next = fwp->next;
		}
		strcpy(fwp->exec_str,exp);
		int re;
		bool success;
		re = expr(exp,&success);
		if(success) fwp->result = re;
		if(head==NULL){
			head = fwp;
			head->next = NULL;
		  printf("successful creat a new watchpoint: NO=%d,exp=%s,result=%d\n",head->NO,head->exec_str,head->result);
		}
		else{
			fwp->next = head->next;
			head->next = fwp;
			printf("successful creat a new watchpoint: NO=%d,exp=%s,result=%d\n",fwp->NO,fwp->exec_str,fwp->result);
		}
	}
	return fwp;
}

void free_wp(int i){
	WP *pre = head,*p = head;
	WP *wp;
	if(head->NO == i){
		wp = head;
		head = head->next;
		printf("successful delete the watchpoint NO=%d\n",wp->NO);
	}
	else{
		pre = head;
		p = pre->next;
		while(p){
			if(p->NO == i){
				pre->next = p->next;
				printf("successful delete the watchpoint NO=%d\n",p->NO);
				break;
			}
			else {
				pre = pre->next;
				p = pre->next;
			}
		}
		wp = p;
	}

	//free_
	if(free_ == NULL){
		free_ = wp;
		free_->next = NULL;
	}
	else{
		wp->next = free_->next;
		free_->next = wp;
	}
}


void print_w(){
	WP *p = head;
	if(p==NULL) return;
	printf("NO    what                result   \n");
	while(p){
		printf("%-6d%-20s%d\n",p->NO,p->exec_str,p->result);
		p=p->next;
	}
}


bool check_points(){
	WP *p =head;
	bool success;
	int re;
	if(p==NULL) return false;
	else{
		while(p){
		Log("wp NO=%d,expr=%s",p->NO,p->exec_str);
			re = expr(p->exec_str,&success);
			if(p->result != re){
				printf("assert at wp NO=%d,expr=%s,result=%d\n",p->NO,p->exec_str,re);
				return true;
			}
			p = p->next;
		}
		return false;
	}
}
