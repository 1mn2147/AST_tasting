#include <stdio.h>
#include <memory.h>
#include "json_c.c"
#include <string.h>

typedef struct {
	char id[10];
	char type[20];
} Batter;

int main(void)
{   
    const char *str = "{\"id\": \"0001\",\"type\": \"donut\",\"name\": \"Cake\",\"ppu\": 0.55,\"batters\": {\"batter\": [{\"id\": \"1001\", \"type\": \"Regular\"},{\"id\": \"1002\", \"type\": \"Chocolate\"},{\"id\": \"1003\", \"type\": \"Blueberry\"},{\"id\": \"1004\", \"type\": \"Devil's Food\"}]}, \"topping\": [{\"id\": \"5001\", \"type\": \"None\"},{\"id\": \"5002\", \"type\": \"Glazed\"},{\"id\": \"5005\", \"type\": \"Sugar\"},{\"id\": \"5007\", \"type\": \"Powdered Sugar\"},{\"id\": \"5006\", \"type\": \"Chocolate with Sprinkles\"},{\"id\": \"5003\", \"type\": \"Chocolate\"},{\"id\": \"5004\", \"type\": \"Maple\"}]}";
	//Creation and assignment of a json_value
	//Dynamic allocation used
	json_value json = json_create(str);

	json_value json2 = json_get(json, "batters");
	json_value json3 = json_get(json2, "batter"); // 어레이

	int len = json_len(json3);

	Batter *Batt_array = (Batter*)malloc(sizeof(Batter) * len);

	json_print(json);
	for(int i = 0; i < len; i++){
		json_value ob = json_get(json3,i); // i번째 오브젝트 가져오기
		char *id = json_get_string(ob, "id"); // 오브젝트 스트링으로 저장
		char *type = json_get_string(ob, "type");
		strcpy(Batt_array[i].id,id);
		strcpy(Batt_array[i].type,type);
	}

	for(int j = 0; j < json_len(json3);j++){
		printf("id: %s, type: %s\n",Batt_array[j].id, Batt_array[j].type);
	}
	return 0;
}

