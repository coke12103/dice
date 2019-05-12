#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <regex.h>
#include <string.h>

long get_rand(long count, long seed);
int get_long_digit(long count_i);

int main(int argc, char **argv){
  long result = 0;

  // 初期値
  long rand_base = 6;
  long rand_count = 1;
  long rand_plus = 0;
  char *input;

  // これargv[0]に1d6とか入ってたらどうなるんだろう...
  // ↑ダイス振れるわ。ウケる。
  if(argc == 1){
    input = argv[0];
  }else{
    input = argv[1];
  }

  // コードだけ抽出する
  char dice_code[sizeof(input)/sizeof(input[0])];

  // 1d6+1d4みたいな表記どう対応しましょう？
  const char regexp[] = "([0-9]+)([Dd]{1})([0-9]+)((\\+([0-9]+))?)";
  regex_t regexp_buf;

  if(regcomp(&regexp_buf, regexp, REG_EXTENDED|REG_NEWLINE) != 0){
    printf("regex compile failed");
    return 1;
  }

  regmatch_t reg_match[7];
  int size = sizeof(reg_match)/sizeof(regmatch_t);

  // 正規表現に一致したら色々上書きする
  if(regexec(&regexp_buf, input, size, &reg_match[0], 0) == 0){
    for(int i = 0; i < size; ++i){
      int start_index = reg_match[i].rm_so;
      int end_index = reg_match[i].rm_eo;

      char str[32] = "";
      switch(i){
        case 0:
        case 1:
        case 3:
        case 6:
          for(int ii = start_index; ii < end_index; ++ii){
            str[ii - start_index] = input[ii];
          }

          switch(i){
            case 0:
              strcpy(dice_code, str);
              break;
            case 1:
              rand_count = strtol(str, NULL, 10);
              break;
            case 3:
              rand_base = strtol(str, NULL, 10);
              break;
            case 6:
              rand_plus = strtol(str, NULL, 10);
          }
          break;
      }
    }
  }

  // 上書きされててもここで宣言すれば多分問題ないっしょ
  long rand_arr[rand_count];

  for(int i = 0; i < rand_count; ++i){
    long random = 0;
    random = get_rand(rand_base, i);
    result = result + random;
    rand_arr[i] = random;
  }

  // これ計算合ってるのかわからんけどオーバーフローしたことないから多分問題ないんじゃないかな
  char rand_arr_text[((sizeof(rand_arr) / sizeof(rand_arr[0])) * get_long_digit(rand_base)) + (sizeof(rand_arr) / sizeof(rand_arr[0])) + 1];

  // 100回以上振るならいっぱいにする
  // まずそんなに振らんやろってのはあるが
  if(rand_count > 100){
    strcat(rand_arr_text, "いっぱい");
  }else{
    for(int i = 0; i < rand_count; ++i){
      char buf[32];
      snprintf(buf, 32, "%ld", rand_arr[i]);
      if(!(i == 0)){
        strcat(rand_arr_text, ",");
      }else{
        rand_arr_text[0] = '\0';
      }
      strcat(rand_arr_text, buf);
    }
  }

  // 結構雑に実装した感がある
  if(rand_count >= 2){
    if(rand_plus != 0){
      printf("(%s) -> %ld[%s]+%ld -> ", dice_code, result, rand_arr_text, rand_plus);
      result = result + rand_plus;
      printf("%ld\n", result);
    }else{
      printf("(%s) -> %ld[%s] -> %ld", dice_code, result, rand_arr_text, result);
    }
  }else{
    if(rand_plus != 0){
      printf("(%s) -> %ld+%ld -> ", dice_code, result, rand_plus);
      result = result + rand_plus;
      printf("%ld\n", result);
    }else{
      printf("(%s) -> %ld", dice_code, result);
    }
  }

  regfree(&regexp_buf);

  return 0;
}

// 乱数。これでも微妙にガバガバなので困る。
long get_rand(long count, long seed){
  long s = ((seed + 1) * 100) / 3;
  srand(time(NULL) * s);

  return rand()%count+1;
}

// 桁数出してくれるすごいやつ
int get_long_digit(long count_i){
  int dig = 0;
  while(count_i != 0){
    count_i = count_i /10;
    ++dig;
  }
  return dig;
}
