#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_c.c"

// 박진석_3871
void print_fun_name(json_value ext) 
{
    for(int i = 0; i < json_len(ext); i++)
    {
        json_value obj = json_get_from_array((json_array *)ext.value, i);

        if(strcmp(json_get_string(obj, "_nodetype"), "FuncDef") == 0)
        {
            json_value decl = json_get(obj, "decl");
            json_value name = json_get(decl, "name");
                
            printf("Func name is : %s\n", name.value);
        }
    }
}

void fun2(json_value ext) { }
void fun3(json_value ext) { }
void fun4(json_value ext) { }

// 박윤_4538
void print_fun_param_info(json_value ext)
{
    json_value ext_in = json_get_from_array((json_array *)ext.value, 0);

	for (int i = 0; i < json_len(ext_in); i++) {
		json_value json1 = json_get(ext_in, i);

		if (json1.type == JSON_OBJECT) { 
			char* nodetype = json_get_string(json1, "_nodetype");
			if (strcmp(nodetype, "Decl") == 0) {
				json_value params_json = json_get(json_get(json_get(json1, "type"), "args"), "params");
				for (int j = 0; j < json_len(params_json); j++) {
					json_value param = json_get(params_json, j);
					char *type = json_get_string(json_get(json_get(json_get(json_get(param, "type"), "type"), "names"), 0));
					printf("name: %s, type: %s\n", json_get_string(param, "name"), type);
				}
			}
			free(nodetype);
		}
	}
}

int main() {
    FILE *file = fopen("red_black_terr.c.json", "r");

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_string = (char *)malloc(file_size + 1);

    size_t read_size = fread(json_string, 1, file_size, file);
    json_string[file_size] = '\0';

    fclose(file);

    json_value json = json_create(json_string);
    
    free(json_string);

    json_value ext = json_get(json, "ext");

    // 함수 이름 출력
    print_fun_name(ext);

    json_free(json);

    return 0;
}