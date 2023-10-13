# ASTparser

### main
```
int main() {
    FILE *file = fopen("red_black_tree.c.json", "r");
    //FILE *file = fopen("linked_list_operations.c.json", "r");

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

    print_fun_name(ext);
    print_count_fun(ext);
    print_count_if(ext);
    print_fun_returnType(ext);
    print_fun_param_info(ext);
    
    json_free(json);

    return 0;
}
```
`FILE *file = fopen("red_black_tree.c.json", "r")`
-  fopen 함수를 사용하여 "red_black_terr.c.json" 파일을 읽기 모드("r")로 엽니다.

`fseek(file, 0, SEEK_END)`, `long file_size = ftell(file)`
-  fseek 함수를 사용하여 파일 끝으로 이동한 다음 ftell 함수를 사용하여 파일의 크기를 계산하고 file_size 변수에 저장합니다.

`char *json_string = (char *)malloc(file_size + 1);`
-  파일을 읽을 메모리를 동적으로 할당합니다.

`size_t read_size = fread(json_string, 1, file_size, file)`
-  fread 함수를 사용하여 파일 크기만큼 파일의 내용을 읽고, json_string 버퍼에 저장합니다.

`json_value json = json_create(json_string)`
-  json_create 함수를 호출하여 json_string을 기반으로 JSON 데이터를 파싱하고, 파싱된 데이터를 json 변수에 저장합니다.

`json_value ext = json_get(json, "ext")`
-  JSON 데이터에서 "ext" 키에 해당하는 값을 찾고, 저장합니다.

### void print_fun_name(json_value ext)
```
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
```
`json_value obj = json_get_from_array((json_array *)ext.value, i)`
-  json_array에서 i번째 요소(obj)를 가져옵니다.

`if(strcmp(json_get_string(obj, "_nodetype"), "FuncDef") == 0)`
- 객체(obj)의 "_nodetype" 키의 값과 "FuncDef" 문자열을 비교하여 일치하는 JSON 객체를 찾습니다.

`json_value decl = json_get(obj, "decl")`, `json_value name = json_get(decl, "name")`
- "FuncDef"를 나타내는 JSON 객체(obj)가 발견되면 해당 객체에서 "decl" 키를 사용하여 JSON 객체가져 옵니다.
- 가져온 JSON 객체에서 "name" 키를 사용하여 함수의 이름을 가져옵니다.

### 각자 함수명2

### 컴파일 및 실행 결과
컴파일러 버전
```gcc -o ASTparser ASTparser.c
