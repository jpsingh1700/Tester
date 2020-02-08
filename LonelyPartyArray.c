#include "LonelyPartyArray.h"
#include <stdio.h>
#include <stdlib.h>

LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
  LPA *LonelyP = NULL;
  int i;
  if (num_fragments<=0||fragment_length<=0)
  {
    return NULL;
  }
  LonelyP = malloc(sizeof(LPA));
  if (LonelyP == NULL)
  {
    return NULL;
  }
  LonelyP->num_fragments = num_fragments;
  LonelyP->fragment_length = fragment_length;
  LonelyP->size = 0;
  LonelyP->num_active_fragments = 0;
  LonelyP->fragments = (int**) malloc(LonelyP->num_fragments*sizeof(int*));
  for (i=0;i<LonelyP->num_fragments;i++)
  {
    LonelyP->fragments[i] = NULL;
  }
  LonelyP->fragment_sizes = (int*)malloc(LonelyP->num_fragments*sizeof(int));
  for (i=0;i<LonelyP->num_fragments;i++)
  {
    LonelyP->fragment_sizes[i] = 0;
  }

  printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n", LonelyP->num_fragments * LonelyP->fragment_length, LonelyP->num_fragments);
  return LonelyP;
}

LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *LonelyP)
{
  if (LonelyP != NULL)
  {
    int i;
    for(i=0; i<LonelyP->num_fragments; i++)
    {
      if (LonelyP->fragments[i] != NULL)
        free(LonelyP->fragments[i]);
        //free(LonelyP->fragment_sizes[i]);
    }
    free(LonelyP->fragments);
    free(LonelyP->fragment_sizes);
    //free(LonelyP->fragment_length);
    //free(LonelyP->size);
    //free(LonelyP->num_active_fragments);
    //free(LonelyP->num_fragments);
    free(LonelyP);

    printf("-> The LonelyPartyArray has returned to the void.\n");
    return NULL;
  }
  return NULL;
}

int set(LonelyPartyArray *LonelyP, int index, int key)
{
  int frag_num;
  int cell;
  int high, low;
  int i;

  if (LonelyP == NULL)
  {
    printf("-> Bloop! NULL pointer detected in set().\n");
    return LPA_FAILURE;
  }

  frag_num = index / (LonelyP->fragment_length);
  cell = index % (LonelyP->fragment_length);

  if (index < 0 || index > ((LonelyP->num_fragments) * (LonelyP->fragment_length)) - 1)
  {
    printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, frag_num, cell);
    return LPA_FAILURE;
  }

  if (LonelyP->fragments[frag_num] == NULL)
  {
    low = (LonelyP->fragment_length) * frag_num;
    high = ((LonelyP->fragment_length) * (frag_num+1)) - 1;
    LonelyP->fragments[frag_num] = malloc(LonelyP->fragment_length * sizeof(int *));

    for(i=0;i<LonelyP->fragment_length;i++)
    {
      LonelyP->fragments[frag_num][i] = UNUSED;
    }
    LonelyP->fragments[frag_num][cell] = key;
    LonelyP->size++;
    LonelyP->fragment_sizes[frag_num]++;

    if (LonelyP->fragments[frag_num] == NULL)
      return LPA_FAILURE;

    printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n", frag_num, LonelyP->fragment_length, low, high);
    LonelyP->num_active_fragments++;
    return LPA_SUCCESS;
  }
  else if(LonelyP->fragments[frag_num][cell]==UNUSED)
  {
    LonelyP->fragments[frag_num][cell] = key;
    LonelyP->size++;
    LonelyP->fragment_sizes[frag_num]++;
  }
  else if(LonelyP->fragments[frag_num][cell]!=UNUSED)
  {
    LonelyP->fragments[frag_num][cell] = key;
  }
  return LPA_SUCCESS;
}
int get(LonelyPartyArray *LonelyP, int index)
{
  int frag_num, cell;

  if (LonelyP == NULL)
  {
    printf("-> Bloop! NULL pointer detected in get().\n");
    return LPA_FAILURE;
  }

  frag_num = index / (LonelyP->fragment_length);
  cell = index % (LonelyP->fragment_length);

  if (index < 0 || index > ((LonelyP->num_fragments) * (LonelyP->fragment_length)) - 1)
  {
    printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, frag_num, cell);
    return LPA_FAILURE;
  }
  if (LonelyP->fragments[frag_num] == NULL)
    return UNUSED;
  return LonelyP->fragments[frag_num][cell];
}
int delete(LonelyPartyArray *LonelyP, int index)
{
  int frag_num, cell;
  int high, low;

  if (LonelyP == NULL)
  {
    printf("-> Bloop! NULL pointer detected in delete().\n");
    return LPA_FAILURE;
  }

  frag_num = index / (LonelyP->fragment_length);
  cell = index % (LonelyP->fragment_length);

  if (index < 0 || index > ((LonelyP->num_fragments) * (LonelyP->fragment_length)) - 1)
  {
    printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n", index, frag_num, cell);
    return LPA_FAILURE;
  }

  if (LonelyP->fragments[frag_num] == NULL || LonelyP->fragments[frag_num][cell] == UNUSED)
    return LPA_FAILURE;
  if(LonelyP->fragments[frag_num][cell] != UNUSED)
  {
    LonelyP->fragments[frag_num][cell] = UNUSED;
    LonelyP->size--;
    LonelyP->fragment_sizes[frag_num]--;
  }

  if (LonelyP->fragment_sizes[frag_num] == 0)
  {
    free(LonelyP->fragments[frag_num]);
    LonelyP->fragments[frag_num] = NULL;
    LonelyP->num_active_fragments--;
    low = (LonelyP->fragment_length) * frag_num;
    high = (LonelyP->fragment_length) * (frag_num + 1) - 1;
    printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n",frag_num, LonelyP->fragment_length, low, high);
    return LPA_SUCCESS;
  }
  return LPA_SUCCESS;
}
int containsKey(LonelyPartyArray *LonelyP, int key)
{
  int i, j;

  if (LonelyP == NULL)
    return 0;

  for(i=0; i<LonelyP->num_fragments; i++)
  {
    if (LonelyP->fragments[i] != NULL)
    {
      for(j=0; j<LonelyP->fragment_length; j++)
      {
        if (LonelyP->fragments[i][j] == key)
          return 1;
      }
    }
  }
  return 0;
}
int isSet(LonelyPartyArray *LonelyP, int index)
{
  int frag_num, cell;

  if (LonelyP == NULL)
    return 0;

  frag_num = index / (LonelyP->fragment_length);
  cell = index % (LonelyP->fragment_length);

  if (index < 0 || index > ((LonelyP->num_fragments) * (LonelyP->fragment_length)) - 1)
    return 0;

  if (LonelyP->fragments[frag_num] == NULL)
    return 0;

  if (LonelyP->fragments[frag_num][cell] == UNUSED)
    return 0;

  return 1;
}
int printIfValid(LonelyPartyArray *LonelyP, int index)
{
  int frag_num, cell;

  if (LonelyP == NULL)
    return LPA_FAILURE;

  frag_num = index / (LonelyP->fragment_length);
  cell = index % (LonelyP->fragment_length);

  if (index < 0 || index > ((LonelyP->num_fragments) * (LonelyP->fragment_length)) - 1)
    return LPA_FAILURE;

  if (LonelyP->fragments[frag_num]== NULL)
    return LPA_FAILURE;

  if (LonelyP->fragments[frag_num][cell] == UNUSED)
    return LPA_FAILURE;

  printf("%d\n", LonelyP->fragments[frag_num][cell]);
  return LPA_SUCCESS;
}
LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *LonelyP)
{
  int i;
  int cap;

  if (LonelyP == NULL)
  {
    printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().");
    return LonelyP;
  }

  for(i=0; i<LonelyP->num_fragments; i++)
  {
    if (LonelyP->fragments[i] != NULL)
    {
      free(LonelyP->fragments[i]);
      LonelyP->fragments[i] = NULL;
    }
    LonelyP->fragment_sizes[i] = 0;
  }

  LonelyP->size = 0;
  LonelyP->num_active_fragments = 0;

  cap = LonelyP->num_fragments * LonelyP->fragment_length;

  printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n", cap, LonelyP->num_fragments);

  return LonelyP;
}
int getSize(LonelyPartyArray *LonelyP)
{
  if (LonelyP == NULL)
    return -1;
  return LonelyP->size;
}
int getCapacity(LonelyPartyArray *LonelyP)
{
  int cap;

  if (LonelyP == NULL)
    return -1;

  cap = LonelyP->num_fragments * LonelyP->fragment_length;
  return cap;
}
int getAllocatedCellCount(LonelyPartyArray *LonelyP)
{
  int alloc=0;
  int i,j;

  if (LonelyP == NULL)
    return -1;

  for (i = 0; i < LonelyP->num_fragments; i++)
	{
		if (LonelyP->fragments[i] != NULL)
		{
			alloc += LonelyP->fragment_length;
		}
	}
  return alloc;
}
long long unsigned int getArraySizeInBytes(LonelyPartyArray *LonelyP)
{

  return (LonelyP->num_fragments * LonelyP->fragment_length) * sizeof(int);
}
long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *LonelyP)
{
  long long unsigned int size = 0;
  int i, j;

  if (LonelyP == NULL)
    return 0;

  size+= sizeof(LPA*);
  size+= sizeof(LPA);

  for(i=0; i<LonelyP->num_fragments; i++)
  {
    size+= sizeof(LonelyP->fragments[i]);
    size+= sizeof(LonelyP->fragment_sizes[i]);
    if (LonelyP->fragments[i] != NULL)
    {
      for (j=0; j<LonelyP->fragment_length; j++)
      {
        size+= sizeof(LonelyP->fragments[i][j]);
      }
    }
  }
  return size;
}
double difficultyRating(void)
{
  return 5;
}
double hoursSpent(void)
{
  return 30;
}
