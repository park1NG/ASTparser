# ASTparser

### Source File
1. [red_black_tree.c](/file/red_black_tree.c)
   [linked_list_operations.c](/file/linked_list_operations.c)
2. [AstParser](/AstParser)
3. [generate_ast.py](/generate_ast.py)

</br>
</br>

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

</br>
</br>

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

</br>
</br>

### print_fun_returnType
```
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
```
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

</br>
</br>

### count_if_nodetype
```
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
```
`if (ext.type == JSON_OBJECT)`
- ext가 JSON_OBJECT 유형인지 확인합니다.
  
` char* nodetype = json_get_string(ext, "_nodetype");`
- ext에서 _nodetype 속성을 추출합니다.

```
if (nodetype != NULL && strcmp(nodetype, "If") == 0) {
            count++;
}
```
- 만약 "_nodetype" 속성이 존재하고 그 값이 "If"와 같다면, 카운트를 1 증가시킵니다.

```
for(int i = 0; i < json_len(ext); i++) {
            json_value child = json_get(ext, i);
            count += count_if_nodetype(child);
}
```
- ext의 모든 하위 노드에 대해 재귀적으로 호출하여 카운트를 누적합니다.

```
else if (ext.type == JSON_ARRAY) {
        for(int i=0; i<json_len(ext); i++) {
            json_value child = json_get_from_array((json_array *)ext.value, i);
            count += count_if_nodetype(child);
        }
}
```
- 만약 'ext'가 JSON_ARRAY 유형이라면, ext의 모든 하위 노드에 대해 재귀적으로 호출하여 카운트를 누적합니다.

`return count;`
- 최종 카운트를 값을 반환합니다.

</br>

### print_count_if
```
int print_count_if(json_value ext) {
    int functionCount = 0;

    for(int i = 0; i < json_len(ext); i++) {
        json_value obj = json_get_from_array((json_array *)ext.value, i);

        if (strcmp(json_get_string(obj, "_nodetype"), "FuncDef") == 0) {
            int ifCount = count_if_nodetype(obj);
            printf("Function: %s, If Count: %d\n", json_get_string(json_get(json_get(obj, "decl")), "name"), ifCount);
        }
    }
}
```
`json_value obj = json_get_from_array((json_array *)ext.value, i);`
- json_array에서 i번째 요소(obj)를 가져옵니다.

`if (strcmp(json_get_string(obj, "_nodetype"), "FuncDef") == 0)`
- 객체(obj)의 "_nodetype" 키의 값과 "FuncDef" 문자열을 비교하여 일치하는 JSON 객체를 찾습니다.

`int ifCount = count_if_nodetype(obj);`
- "FuncDef"인 경우, count_if_nodetype 함수를 호출하여 "If" 노드의 개수를 얻습니다.

`printf("Function: %s, If Count: %d\n", json_get_string(json_get(json_get(obj, "decl")), "name"), ifCount);`
- 함수 이름과 해당 함수 내의 "If" 노드 개수를 출력합니다.

</br>
</br>

### print_count_fun
```
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

    return functionCount;
}
```

</br>

```
for(int i = 0; i < json_len(ext); i++)
{
        json_value obj = json_get_from_array((json_array *)ext.value, i);
        if(strcmp(json_get_string(obj, "_nodetype"), "FuncDef") == 0)
	...
}
```
- ext길이만큼 반복문을 실행하면서, strcmp함수를 통해서 함수인지 아닌지 비교하는 과정을 거칩니다.

```
if(strcmp(json_get_string(obj, "_nodetype"), "FuncDef") == 0){
            functionCount++;
};
````
- 만약 함수라면은, ++을 통해서, 함수인것을 카운팅합니다.

`printf("function counts: %d\n", functionCount);`
- 찾은 함수 개수를 출력합니다.

</br>
</br>

### print_fun_returnType
```
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
```
`json_value obj = json_get_from_array((json_array *)ext.value, i)`
-  json_array에서 i번째 요소(obj)를 가져옵니다.

```
char* nodetype = json_get_string(json1, "_nodetype");
if (strcmp(nodetype, "FuncDef") == 0) {
```
- 객체(obj)의 "_nodetype" 키의 값과 "FuncDef" 문자열을 비교하여 일치하는 JSON 객체를 찾습니다.

`if (json_get(json_get(json_get(json1, "decl"), "type"), "args").type == JSON_OBJECT) {`
- json {} > decl {} > type {} > args {} 로 파라미터가 없는지 검사합니다.

```
do {
	json_value type_json1 = json_get(type_json, "type");
	type_json = type_json1;
}while (!(strcmp(json_get_string(type_json, "_nodetype"), "IdentifierType") == 0 || strcmp(json_get_string(type_json, "_nodetype"), "Struct") == 0));
```
- type_json json 객체의 _nodetype key 값의 value가 IdentifierType 가 될 때까지 "type" key를 갖는 json object를 type_json으로 할당합니다.

```
if (strcmp(json_get_string(type_json, "_nodetype"), "Struct") != 0) {
    type = json_get(type_json, "names");
}else {
    type = json_get(type_json, "name");
}
```
- type_json json 객체 에서 구조체라면 "name", 구조체가 아니라면 "names" key를 갖는 json object를 type으로 할당합니다.

```
printf("name: %s, type: ", json_get_string(param, "name"));
json_print(type);
printf("\n");
```
- param json 객체에서 변수의 이름을 얻고, 기존에 구해두었던 type을 출력합니다

</br>
</br>

### 컴파일 및 실행 결과
#### 컴파일러 버전  
```
Apple clang version 14.0.3 (clang-1403.0.22.14.1)
```  
- Windows용 gcc 컴파일러에서 컴파일 후 실행 시 일부 오류 메시지 출력되는 현상이 있습니다. 하지만 출력 결과는 동일합니다.
  
#### 컴파일
```
gcc -o ASTparser ASTparser.c
```
- 실행파일은 레포지토리 내의 ASTparser와 동일합니다.

#### 실행결과  
```
./ASTparser
```
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

function counts: 11

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
- 위의 출력과는 레포지토리 내의 red_black_tree.c.json을 기준으로 작성되었습니다.
