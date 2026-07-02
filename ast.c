#include <stdio.h>
#include <memory.h>
#include "json_c.c"
#include <string.h>
#include <stdlib.h>

json_value json_read(const char * const path) { // json 파일을 열어서 json 구조체로 리턴
    FILE *fp = fopen(path, "r");
    if (fp == NULL){
        printf("파일열기 실패\n");
        return undefined_json;
    }
	else{
        printf("파일열기 성공\n");
    }

    fseek(fp, 0, SEEK_END); //파일크기 측정 후 동적할당
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *json_buffer = (char *)malloc(file_size + 1);
    if (json_buffer == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        fclose(fp);
        return undefined_json;
    }

    fread(json_buffer, 1, file_size, fp);
    json_buffer[file_size] = '\0'; // 문자열 끝 처리

    fclose(fp);

    json_value result = json_create(json_buffer);
    free(json_buffer); 
    return result;
}

int funcCount(json_value extarr){ // json 구조체를 받아 함수 개수 출력
    int count = 0;
    //json_print(extarr);
    int len = json_len(extarr);
    for(int i = 0; i < len; i++){
        json_value current_node = json_get(extarr, i);
        //json_print(current_node);
        char* node_type_str = json_get_string(current_node, "_nodetype");
        //printf("\nnodetypestr: %s\n", node_type_str);
        if(json_is_null(current_node)==0 && strcmp(node_type_str, "FuncDef")==0){
            count++;
        }
    }
    return count;
}

void funcReturnExtract(json_value extarr){ // json 구조체를 받아 함수 개수 출력
    int count = 0;
    //json_print(extarr);
    int len = json_len(extarr);
    for(int i = 0; i < len; i++){
        json_value current_node = json_get(extarr, i);
        //json_print(current_node);
        char* node_type_str = json_get_string(current_node, "_nodetype");
        if(json_is_null(current_node)==0 && strcmp(node_type_str, "FuncDef")==0){
            count++;
            json_value typeDeclType = json_get(current_node, "decl", "type", "type");
            char* typeDeclStr = json_get_string(typeDeclType, "_nodetype");
            if(strcmp(typeDeclStr, "PtrDecl")==0){
                json_value funcType = json_get(current_node, "decl", "type", "type", "type", "type", "names", 0);
                char* funcType_str = json_get_string(funcType);
                printf("%0d번째 함수의 리턴 타입: %s *\n", count, funcType_str);
            }else{
                json_value funcType = json_get(current_node, "decl", "type", "type", "type", "names", 0);
                char* funcType_str = json_get_string(funcType);
                printf("%0d번째 함수의 리턴 타입: %s\n", count, funcType_str);
            }
        }
    }
}

void funcNameExtract(json_value extarr){ // json 구조체를 받아 함수 이름 출력
    int count = 0;
    int len = json_len(extarr);
    for(int i = 0; i < len; i++){
        json_value current_node = json_get(extarr, i);
        char* node_type_str = json_get_string(current_node, "_nodetype");
        if(json_is_null(current_node)==0 && strcmp(node_type_str, "FuncDef")==0){
            count++;
            json_value funcName = json_get(current_node, "decl", "name");
            char* funcName_str = json_get_string(funcName);
            printf("%0d번째 함수의 이름: %s\n", count, funcName_str);
        }
    }
}

void funcParamExtract(json_value extarr){
    int count = 0;
    int len = json_len(extarr);
    for(int i = 0; i < len; i++){
        json_value current_node = json_get(extarr, i);
        char* node_type_str = json_get_string(current_node, "_nodetype");
        if(json_is_null(current_node)==0 && strcmp(node_type_str, "FuncDef")==0){
            count++;
            printf("%0d번째 함수의 파라미터: ", count);
            json_value arg = json_get(current_node, "decl", "type", "args");
            if(json_is_null(arg)){

            }else{
                json_value paramList = json_get(current_node, "decl", "type", "args", "params");
                int paramLen = json_len(paramList);
                for(int j = 0; j < paramLen; j++){
                    json_value param = json_get(paramList, j);
                    char * declType = json_get_string(param, "type", "_nodetype");
                    if(strcmp(declType, "PtrDecl")==0){
                        char * paramName = json_get_string(param, "name");
                        char * paramtype = json_get_string(param, "type", "type", "type", "names", 0);
                        printf("%s %s | ",paramtype, paramName);
                    }
                    else{
                        char * paramName = json_get_string(param, "name");
                        char * paramtype = json_get_string(param, "type", "type", "names", 0);
                        printf("%s %s | ",paramtype, paramName);
                    }
                }
            }
            printf("\n");
        }
    }
}

void funcIfCount(json_value extarr){
    int count = 0;
    int ifcount = 0;
    int len = json_len(extarr);
    for(int i = 0; i < len; i++){
        ifcount = 0;
        json_value current_node = json_get(extarr, i);
        char* node_type_str = json_get_string(current_node, "_nodetype");
        if(json_is_null(current_node)==0 && strcmp(node_type_str, "FuncDef")==0){
            count++;
            json_value bodyList = json_get(current_node, "body", "block_items");
            int bodylen = json_len(bodyList);
            for(int j = 0; j < bodylen; j++){
                json_value bodyNode = json_get(bodyList, j);
                char * bodyNodeType = json_get_string(bodyNode, "_nodetype");
                if(bodyNodeType != NULL && strcmp(bodyNodeType, "If")==0){
                    funcIfRecVisitor(json_get(bodyNode, "iffalse"), &ifcount);
                }
            }
            printf("%d번째 함수의 if 조건 개수는 %d입니다.\n",count, ifcount);
        }
    }
}

void funcIfRecVisitor(json_value IfNode, int *count) {
    if (json_is_null(IfNode)) {
        return;
    }
    char *nodeType = json_get_string(IfNode, "_nodetype");
    if (nodeType != NULL && strcmp(nodeType, "If") == 0) {
        (*count)++;
        json_value nextNode = json_get(IfNode, "iffalse");
        if (json_is_null(nextNode) != 1) {
            funcIfRecVisitor(nextNode, count); 
        }
    }
}

int main(){
    json_value ast = json_read("ast.json");
    json_value ext_arr = json_get(ast,"ext");

    printf("함수의 개수는 %d개 입니다.\n", funcCount(ext_arr));
    funcReturnExtract(ext_arr);
    printf("---\n");
    funcNameExtract(ext_arr);
    printf("---\n");
    funcParamExtract(ext_arr);
    funcIfCount(ext_arr);
}