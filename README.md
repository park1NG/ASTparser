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
- "FuncDef"를 나타내는 JSON 객체(obj)가 발견되면 해당 객체에서 "decl" 키를 사용하여 JSON 객체를 가져 옵니다.
- 가져온 JSON 객체에서 "name" 키를 사용하여 함수의 이름을 가져옵니다.

### print_fun_returnType

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
`json_value obj = json_get_from_array((json_array *)ext.value, i)`
-  json_array에서 i번째 요소(obj)를 가져옵니다.

`if(strcmp(json_get_string(obj, "_nodetype"), "FuncDef") == 0)`
- 객체(obj)의 "_nodetype" 키의 값과 "FuncDef" 문자열을 비교하여 일치하는 JSON 객체를 찾습니다.

```
json_value first_type = json_get(decl, "type");
json_value second_type = json_get(first_type, "type");
```
- decl json 객체 에서 "type" key를 갖는 json object를 first_type으로 할당합니다.
- 같은 과정을 한번 더 거쳐서 second_type으로 할당합니다.

`if(strcmp(json_get_string(second_type, "_nodetype"), "PtrDecl") == 0)`
- second_type 객체에서 _nodetype의 value가 "PtrDecl" 인 경우 조건문 내부에 진입한다
- 해당 조건은 함수의 리턴타입이 pointer(*)를 반환하는 경우를 위한 조건입니다.

`json_value pname = json_get(pfinal_type, "name");`
- pfinal_type json 객체 에서 "name" key를 갖는 json object를 pname으로 할당합니다.

`if(pname.value == 0)`
- 해당 객체에서 name이 아닌 names 라는 key를 가질 경우를 위한 조건입니다.

`if (json_len(test_type) == 0 && strcmp(json_get_string(third_type, "_nodetype"), "IdentifierType") == 0)`
- 위의 조건문 모두에 해당하지 않는 경우에 대한 조건이고, 이때 _nodetype 키의 value가 IdentifierType인 경우를 위한 조건입니다.
- 해당 경우까지 거치면 모든 경우에 대한 함수 리턴 값을 출력할 수 있습니다. 

### 컴파일 및 실행 결과
- 컴파일러 버전  
`Apple clang version 14.0.3 (clang-1403.0.22.14.1)`  
Windows용 gcc 컴파일러에서 컴파일 후 실행 시 일부 오류 메시지 출력되는 현상이 있습니다. 하지만 출력 결과는 동일합니다.
- 컴파일
  `gcc -o ASTparser ASTparser.c`
실행파일은 레포지토리 내의 ASTparser와 동일합니다.
- 실행결과  
  `./ASTparser`
```
Func name is : newNode
Func name is : isLeaf
Func name is : leftRotate
Func name is : rightRotate
Func name is : checkNode
Func name is : insertNode
Func name is : checkForCase2
Func name is : deleteNode
Func name is : printInorder
Func name is : checkBlack
Func name is : main
찾은 함수 개수: 11
Function: newNode, If Count: 0
Function: isLeaf, If Count: 1
Function: leftRotate, If Count: 3
Function: rightRotate, If Count: 3
Function: checkNode, If Count: 21
Function: insertNode, If Count: 5
Function: checkForCase2, If Count: 32
Function: deleteNode, If Count: 11
Function: printInorder, If Count: 1
Function: checkBlack, If Count: 2
Function: main, If Count: 2

Function newNode's Return Type is Node *
Function isLeaf's Return Type is int
Function leftRotate's Return Type is Node *
Function rightRotate's Return Type is Node *
Function checkNode's Return Type is void
Function insertNode's Return Type is void
Function checkForCase2's Return Type is void
Function deleteNode's Return Type is void
Function printInorder's Return Type is void
Function checkBlack's Return Type is void
Function main's Return Type is int

name: val, type: int
name: par, type: Node
name: n, type: Node
name: node, type: Node
name: node, type: Node
name: node, type: Node
name: val, type: int
name: root, type: Node
name: toDelete, type: Node
name: delete, type: int
name: fromDirection, type: int
name: root, type: Node
name: val, type: int
name: root, type: Node
name: root, type: Node
name: temp, type: Node
name: c, type: int
```  
위의 출력과는 레포지토리 내의 red_black_tree.c.json을 기준으로 작성되었습니다.
