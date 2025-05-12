/*******************************************************************************
filename : matrix_multi.cpp
version  : 1.0
author   : 김현우
date     : 2025-05-03
repository:
organizations:

description: 이 프로그램은 여러 행렬 파일의 이름을 입력받아 순차적으로 각 파일 안의 행렬의 곱셈을 수행합니다.
             파일이 3개 이상일 경우, 첫 번째 행렬과 두 번째 행렬을 곱한 결과를 세 번째 행렬과 곱하는 방식으로 진행됩니다.
             각 행렬 곱셈은 스레드를 사용하여 병렬로 처리되며 1초 동안 딜레이됩니다.
             결과는 화면에 출력되고 전체 처리 시간이 표시되며, result.txt 파일에 저장됩니다.

입력 형식: 명령줄 인수로 행렬 파일 이름 제공 (예: matrix_multiply.exe matrix1.txt matrix2.txt)

References: CreateThread(), WaitForMultipleObjects(), Sleep(), fopen(), fprintf(), fclose(), clock()

@This file was written by rlagudn04
*******************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 최대 행렬 크기 정의
#define MAX_SIZE 100

// 전역 행렬 버퍼
int A[MAX_SIZE][MAX_SIZE]; // 첫 번째 행렬
int B[MAX_SIZE][MAX_SIZE]; // 두 번째 행렬
int C[MAX_SIZE][MAX_SIZE]; // 결과 행렬
int Ar, Ac, Br, Bc;        // 행렬의 행(row)과 열(column) 크기

// 파일에서 행렬을 읽는 함수
#define DELIM    " \r\t\n" // 구분자 정의
#define MAX_LINE 1000      // 최대 라인 길이
void read_matrix(int M[MAX_SIZE][MAX_SIZE], int& rows, int& cols, const char* filename) {
    // 파일 열기
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "ERROR: Could not open file '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }
    char line[MAX_LINE];
    rows = 0;
    // 파일에서 한 줄씩 읽어 행렬 데이터 처리
    while (fgets(line, MAX_LINE, fp)) {
        char* tok = strtok(line, DELIM);
        cols = 0;
        while (tok) {
            M[rows][cols++] = atoi(tok); // 문자열을 정수로 변환하여 행렬에 저장
            tok = strtok(NULL, DELIM);
        }
        rows++;
    }
    fclose(fp); // 파일 닫기
}

// 스레드 매개변수 구조체
struct ThreadParams {
    int row;       // 계산할 행 인덱스
    int common;    // 내부 차원 길이 (Ac == Br)
};

// 한 행을 계산하는 스레드 함수
DWORD WINAPI MultiplyRow(LPVOID lpParam) {
    ThreadParams* p = (ThreadParams*)lpParam;
    int i = p->row;
    // 결과 행렬 C의 i번째 행 계산
    for (int j = 0; j < Bc; j++) {
        int sum = 0;
        for (int k = 0; k < p->common; k++) {
            sum += A[i][k] * B[k][j]; // 행렬 곱셈 연산
        }
        C[i][j] = sum;
    }
    Sleep(1000); // 1초 지연
    delete p;    // 동적 할당 해제
    return 0;
}

// 전역 A × B → C 행렬 곱셈을 스레드를 사용하여 수행
void multiplyMatrices() {
    HANDLE threads[MAX_SIZE]; // 스레드 핸들 배열
    DWORD threadId;
    // 각 행에 대해 스레드 생성
    for (int i = 0; i < Ar; i++) {
        ThreadParams* param = new ThreadParams{ i, Ac }; // 스레드 매개변수 동적 할당
        threads[i] = CreateThread(
            NULL, 0,
            MultiplyRow,
            param,
            0,
            &threadId
        );
        if (!threads[i]) {
            fprintf(stderr, "ERROR: Thread creation failed for row %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }
    // 모든 스레드가 완료될 때까지 대기
    WaitForMultipleObjects(Ar, threads, TRUE, INFINITE);
    // 스레드 핸들 닫기
    for (int i = 0; i < Ar; i++) {
        CloseHandle(threads[i]);
    }
}

int main() {
    char line[4096];
    // 행렬 파일 이름 입력 요청
    printf("Enter matrix filenames separated by spaces: ");
    // 파일 이름 입력 받기
    if (!fgets(line, sizeof(line), stdin)) {
        fprintf(stderr, "ERROR: Failed to read input.\n");
        return EXIT_FAILURE;
    }

    // 공백과 개행으로 파일 이름 토큰화
    char* filenames[100];
    int n = 0;
    char* tok = strtok(line, DELIM);
    while (tok && n < 100) {
        filenames[n++] = tok;
        tok = strtok(NULL, DELIM);
    }
    // 최소 두 개의 파일이 필요
    if (n < 2) {
        fprintf(stderr, "ERROR: Please provide at least two filenames.\n");
        return EXIT_FAILURE;
    }

    // 시간 측정 시작
    LARGE_INTEGER freq, t1, t2;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t1);

    // 첫 번째 행렬 읽기
    read_matrix(A, Ar, Ac, filenames[0]);

    // 나머지 행렬들과 순차적으로 곱셈 수행
    for (int idx = 1; idx < n; idx++) {
        read_matrix(B, Br, Bc, filenames[idx]);
        // 행렬 차원 호환성 확인
        if (Ac != Br) {
            fprintf(stderr, "ERROR: Dimension mismatch between %s (%dx%d) and %s (%dx%d).\n",
                filenames[idx - 1], Ar, Ac, filenames[idx], Br, Bc);
            return EXIT_FAILURE;
        }
        // 행렬 곱셈 정보 출력
        printf("\nMultiplying '%s' and '%s':\n", filenames[idx - 1], filenames[idx]);

        multiplyMatrices(); // 행렬 곱셈 실행

        // 중간 결과 출력
        printf("Intermediate result (%dx%d):\n", Ar, Bc);
        for (int i = 0; i < Ar; i++) {
            for (int j = 0; j < Bc; j++) {
                printf("%8d", C[i][j]);
            }
            printf("\n");
        }

        // 결과 행렬 C를 A로 복사하여 다음 곱셈 준비
        Ac = Bc;
        for (int i = 0; i < Ar; i++)
            for (int j = 0; j < Ac; j++)
                A[i][j] = C[i][j];
    }

    // 시간 측정 종료
    QueryPerformanceCounter(&t2);
    double elapsed = (double)(t2.QuadPart - t1.QuadPart) / freq.QuadPart;

    // 최종 결과 출력
    printf("\n== Final Result Matrix (%d x %d) ==\n", Ar, Ac);
    for (Ormally, the final result is stored in matrix A
        for (int i = 0; i < Ar; i++) {
            for (int j = 0; j < Ac; j++) {
                printf("%8d", A[i][j]);
            }
            printf("\n");
        }
    // 총 처리 시간 출력
    printf("\nTotal processing time: %.3f seconds\n", elapsed);

        // 결과를 result.txt에 저장
        FILE* ofp = fopen("result.txt", "w");
    if (ofp) {
        for (int i = 0; i < Ar; i++) {
            for (int j = 0; j < Ac; j++) {
                fprintf(ofp, "%d ", A[i][j]);
            }
            fprintf(ofp, "\n");
        }
        fclose(ofp);
        printf("Result has been saved to result.txt.\n");
    }
    else {
        fprintf(stderr, "ERROR: Could not write to result.txt.\n");
    }

    return 0;
}