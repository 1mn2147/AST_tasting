#include <stdio.h>
#include "json_c.c"



int main(void)
{   
    const char *str = "{\"data\":[{\"color\": \"red\", \"value\": \"#f00\"}, {\"color\": \"green\", \"value\": \"#0f0\"}, {\"color\": \"blue\", \"value\": \"#00f\"}, {\"color\": \"cyan\", \"value\": \"#0ff\"}, {\"color\": \"magenta\", \"value\": \"#f0f\"}, {\"color\": \"yellow\", \"value\": \"#ff0\"}, {\"color\": \"black\", \"value\": \"#000\"}]}";
	//Creation and assignment of a json_value
	//Dynamic allocation used
	json_value json = json_create(str);
	
	json_print(json);

	json_value obj = json_get(json, "data");
	int obj_size = json_len(obj);

	printf("%d", obj_size);

	for(int i = 0; i < obj_size; i++){
		json_value ob = json_get(obj,i); // i번째 오브젝트 가져오기
		char *color = json_get_string(ob, "color"); // 오브젝트 스트링으로 저장
		printf("%s\n", color);
	}

	return 0;
}

