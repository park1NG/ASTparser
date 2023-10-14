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
    printf("\n");
}

// 박현재_3918
void print_fun_returnType(json_value ext)
{
    for(int i = 0; i < json_len(ext); i++)
    {
        json_value obj = json_get_from_array((json_array *)ext.value, i);
        
        if(strcmp(json_get_string(obj, "_nodetype"), "FuncDef") == 0)
        {
            json_value decl = json_get(obj, "decl");
            json_value name = json_get(decl, "name");
            printf("Function %s's Return Type is ", name.value);
            json_value first_type = json_get(decl, "type");
            json_value second_type = json_get(first_type, "type");
            if(strcmp(json_get_string(second_type, "_nodetype"), "PtrDecl") == 0)
            {
                json_value ptr_type = json_get(second_type, "type");
                json_value pfinal_type = json_get(ptr_type, "type");
                json_value pname = json_get(pfinal_type, "name");
                if(pname.value == 0)
                {
                    json_value pnames = json_get(pfinal_type, "names");
                    json_print(pnames);
                }
                json_print(pname);
                printf(" *");
            }
            json_value third_type = json_get(second_type, "type");
            json_value test_type = json_get(third_type, "names");
            json_print(test_type);
            
            if (json_len(test_type) == 0 && strcmp(json_get_string(third_type, "_nodetype"), "IdentifierType") == 0)
            {
                json_value final_type = json_get(third_type, "type");
                json_value if_type = json_get(final_type, "names");
                json_print(if_type);
            }
			}   
        printf("\n");
        }
}


//박연준_9205
int count_if_nodetype(json_value ext) {
    int count = 0;
    
    if (ext.type == JSON_OBJECT) {
        char* nodetype = json_get_string(ext, "_nodetype");
        if (nodetype != NULL && strcmp(nodetype, "If") == 0) {
            count++;
        }
        
        for(int i=0; i<json_len(ext); i++) {
            json_value child = json_get(ext, i);
            count += count_if_nodetype(child);
        }
        
    } else if (ext.type == JSON_ARRAY) {
        for(int i=0; i<json_len(ext); i++) {
            json_value child = json_get_from_array((json_array *)ext.value, i);
            count += count_if_nodetype(child);
        }
    }

    return count;
}
int print_count_if(json_value ext) {
    int functionCount = 0;

    for(int i = 0; i < json_len(ext); i++) {
        json_value obj = json_get_from_array((json_array *)ext.value, i);

        if (strcmp(json_get_string(obj, "_nodetype"), "FuncDef") == 0) {
            // 함수 내의 If문 개수를 세는 변수
            int ifCount = count_if_nodetype(obj);
            printf("Function: %s, If Count: %d\n", json_get_string(json_get(json_get(obj, "decl")), "name"), ifCount);
        }
    }
}


// 박솔빈_1442
int print_count_fun(json_value ext) {
    int functionCount = 0;

    for(int i = 0; i < json_len(ext); i++)
    {
        json_value obj = json_get_from_array((json_array *)ext.value, i);

        if(strcmp(json_get_string(obj, "_nodetype"), "FuncDef") == 0){
            functionCount++;
        }
    }
    printf("function counts: %d\n", functionCount);
    printf("\n");
    return functionCount;
}

// 박윤_4538
void print_fun_param_info(json_value ext)
{
	for (int i = 0; i < json_len(ext); i++) {
		json_value json1 = json_get_from_array((json_array *)ext.value, i);

		if (json1.type == JSON_OBJECT) {
			char* nodetype = json_get_string(json1, "_nodetype");
			if (strcmp(nodetype, "FuncDef") == 0) {
                if (json_get(json_get(json_get(json1, "decl"), "type"), "args").type == JSON_OBJECT) {
                    json_value params_json = json_get(json_get(json_get(json_get(json1, "decl"), "type"), "args"), "params");
                    
                    for (int j = 0; j < json_len(params_json); j++) {
                        json_value param = json_get(params_json, j);
                        json_value type_json = param;
                        
                        do {
                            json_value type_json1 = json_get(type_json, "type");
                            type_json = type_json1;
                        }while (!(strcmp(json_get_string(type_json, "_nodetype"), "IdentifierType") == 0 || strcmp(json_get_string(type_json, "_nodetype"), "Struct") == 0));
                        json_value type;
                        if (strcmp(json_get_string(type_json, "_nodetype"), "Struct") != 0) {
                            type = json_get(type_json, "names");
                        }else {
                            type = json_get(type_json, "name");
                        }
                        
                        printf("name: %s, type: ", json_get_string(param, "name"));
                        json_print(type);
                        printf("\n");
                    }
                }
			}
            // if (nodetype != NULL) free(nodetype);
		}
	}
}

int main() {
    FILE *file = fopen("red_black_tree.c.json", "r");

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

    //함수 개수 출력
    print_count_fun(ext);
	
    //If문 개수 출력
    print_count_if(ext);
    
    // 함수 리턴타입 출력
    print_fun_returnType(ext);

    //함수 파라미터 타입, 변수명 출력
    print_fun_param_info(ext);
    
    json_free(json);

    return 0;
}
