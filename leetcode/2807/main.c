#include <stdlib.h>
#include <stdio.h>

struct ListNode
{
  int val;
  struct ListNode *next;
};

int calculate_greatest_common_divisor(int m, int n)
{
  if (m == 0)
  {
    return n;
  }
  else if (n == 0)
  {
    return m;
  }
  else if (m == n)
  {
    return m;
  }
  else if (m == 1 || n == 1)
  {
    return 1;
  }
  else if (m % 2 == 0 && n % 2 == 0)
  {
    return 2 * calculate_greatest_common_divisor(m >> 1, n >> 1);
  }
  else if (m % 2 == 0 && n % 2 != 0)
  {
    return calculate_greatest_common_divisor(m >> 1, n);
  }
  else if (m % 2 != 0 && n % 2 == 0)
  {
    return calculate_greatest_common_divisor(m, n >> 1);
  }
  else if (m % 2 != 0 && n % 2 != 0 && n > m)
  {
    return calculate_greatest_common_divisor(m, (n - m) >> 1);
  }
  else if (m % 2 != 0 && n % 2 != 0 && n < m)
  {
    return calculate_greatest_common_divisor((m - n) >> 1, n);
  }

  return 1;
}

struct ListNode *insert_greatest_common_divisors(struct ListNode *head)
{
  struct ListNode *current_node = head;

  while (current_node->next != NULL)
  {

    struct ListNode *new_node = (struct ListNode *)malloc(sizeof(struct ListNode));
    if (new_node == NULL)
    {
      printf("Memory allocation failed.");
      return NULL;
    }
    new_node->val = calculate_greatest_common_divisor(current_node->val, current_node->next->val);
    new_node->next = current_node->next;
    current_node->next = new_node;

    current_node = new_node->next;
  }

  return head;
}

struct ListNode *to_list_node(int array[], int size)
{
  if (size < 1)
  {
    return NULL;
  }

  struct ListNode *head = (struct ListNode *)malloc(sizeof(struct ListNode));
  struct ListNode *current_node = head;

  for (int i = 0; i < size; i++)
  {
    current_node->val = array[i];

    if (i + 1 < size)
    {
      current_node->next = (struct ListNode *)malloc(sizeof(struct ListNode));
    }
    else
    {
      current_node->next = NULL;
      return head;
    }

    current_node = current_node->next;
  }
}

void print_list(struct ListNode *head)
{
  struct ListNode *current_node = head;
  while (current_node != NULL)
  {
    if (current_node->next != NULL)
    {
      printf("%d, ", current_node->val);
    }
    else
    {
      printf("%d", current_node->val);
    }
    current_node = current_node->next;
  }
  printf("\n");
}

int main(void)
{
  int values[] = {18, 6, 10, 3};
  struct ListNode *head = to_list_node(values, sizeof(values) / sizeof(int));
  print_list(head);
  head = insert_greatest_common_divisors(head);
  print_list(head);
}