#pragma once
#include <mutex>

struct Node {
	int value;
	Node* next;
	std::mutex* node_mutex;
};

class FineGrainedQueue {
	Node* head;
	std::mutex* queue_mutex;
	std::mutex* pos_mutex;

	void insertIntoMiddle(int value, int pos) {

		Node* prev, * cur;
		
		pos_mutex->lock();
		int cur_pos = 2;
		
		queue_mutex->lock();

		prev = this->head;
		cur = this->head->next;
		
		prev->node_mutex->lock();

		queue_mutex->unlock();

		if (cur != nullptr) {
			
			cur->node_mutex->lock();
			
			while (cur != nullptr) {
				if (cur_pos == pos) {
					Node* new_node;
					new_node->next = cur;
					new_node->value = value;
					prev->next = new_node;
					prev->node_mutex->unlock();
					cur->node_mutex->unlock();
					pos_mutex->unlock();
					return;
				}
				Node* old_prev = prev;
				prev = cur;
				cur = cur->next;
				old_prev->node_mutex->unlock();
				if (cur != nullptr) {
					cur->node_mutex->lock();
					cur_pos += 1;
				}
				else if (cur == nullptr) {
					Node* new_node;
					new_node->next = nullptr;
					new_node->value = value;
					prev->next = new_node;
					prev->node_mutex->unlock();
					pos_mutex->unlock();
					return;
				}
			}
		}
		else if (cur == nullptr) {
			Node* new_node;
			new_node->next = nullptr;
			new_node->value = value;
			prev->next = new_node;
			prev->node_mutex->unlock();
			pos_mutex->unlock();
			return;
		}
	
	}

};
