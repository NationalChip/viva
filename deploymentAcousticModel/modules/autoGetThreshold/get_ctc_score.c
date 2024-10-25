#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    char kws_words[70];
    short labels[20];
    int  label_length;
} LVP_KWS_PARAM;

typedef struct {
    unsigned int    count;
    LVP_KWS_PARAM  *kws_param_list;
} LVP_KWS_PARAM_LIST;

#define LOG_TAG "[LVP_CTC_GXDECODER]"
#ifndef CONFIG_KWS_MODEL_DECODER_WIN_LENGTH
# define CONFIG_KWS_MODEL_DECODER_WIN_LENGTH 38
#endif

#define OUTPUT_FILE_NUM 100
FILE* fp_score_output[OUTPUT_FILE_NUM] = {NULL};
LVP_KWS_PARAM_LIST g_kws_list;
LVP_KWS_PARAM g_kws_param_list[500];
static int s_ctc_index = 0;
static int s_kws_list_cnt = 0;
static float s_ctc_decoder_window[CONFIG_KWS_MODEL_DECODER_WIN_LENGTH][CONFIG_KWS_MODEL_OUTPUT_LENGTH] __attribute__((aligned(16)));

extern float LvpFastCtcBlockScorePlus(float *probs, int T1, float *probs2, int T2, int alphabet_size, int blank, unsigned short *labels, int labels_length, int *score_index);

static int PrepareData(float *rnn_out)
{
    int idx = s_ctc_index % CONFIG_KWS_MODEL_DECODER_WIN_LENGTH;
    memcpy(&s_ctc_decoder_window[idx], rnn_out, CONFIG_KWS_MODEL_OUTPUT_LENGTH * sizeof(float));
    s_ctc_index++;

    return CONFIG_KWS_MODEL_DECODER_WIN_LENGTH;
}

static int _LvpDoGroupScore(int valid_frame_num, int pid, int cpu_count)
{
    for (int i = pid; i < g_kws_list.count; i+=cpu_count) {
        if (valid_frame_num < sizeof(g_kws_list.kws_param_list[i].label_length))
            continue;

        unsigned short *labels = (unsigned short *)&g_kws_list.kws_param_list[i].labels[0];
        int label_length = g_kws_list.kws_param_list[i].label_length;
        int idx = s_ctc_index % CONFIG_KWS_MODEL_DECODER_WIN_LENGTH;
        int start_index = 0;

        float ctc_score = LvpFastCtcBlockScorePlus(&s_ctc_decoder_window[idx][0]
                        , valid_frame_num - idx
                        , &s_ctc_decoder_window[0][0]
                        , idx
                        , CONFIG_KWS_MODEL_OUTPUT_LENGTH
                        , CONFIG_KWS_MODEL_OUTPUT_LENGTH - 1
                        , labels
                        , label_length
                        , &start_index);

        /* 把 ctc 的打分分数写入文件 */
        if (fp_score_output != NULL) {
            fprintf(fp_score_output[pid], "%s[%d]\n", g_kws_list.kws_param_list[i].kws_words, (int)(ctc_score * 10));
            // printf("%s[%d]\n",g_kws_list.kws_param_list[i].kws_words,(int)(ctc_score*10));
            // fflush(fp_score_output);
        }
    }
    fflush(fp_score_output[pid]);

    return 0;
}

void LvpCloseScoreFile(void)
{
    for (int i = 0; i < OUTPUT_FILE_NUM; i++) {
        if (fp_score_output[i] != NULL) {
            fclose(fp_score_output[i]);
            // printf(LOG_TAG "Kws Score Generated complete!\n");
        }

    }
}

int LvpDoKwsScore(float *rnn_out, int pid, int cpu_count)
{
    int valid_frame_num = PrepareData(rnn_out);

    _LvpDoGroupScore(valid_frame_num, pid, cpu_count);

    return 0;
}

int LvpInitKwsParamList(char *kws_word, short *labels, int label_length)
{
    memcpy(g_kws_param_list[s_kws_list_cnt].kws_words, kws_word, strlen(kws_word));

    for (int i = 0; i < label_length; i++) {
        g_kws_param_list[s_kws_list_cnt].labels[i] = labels[i];
    }
    g_kws_param_list[s_kws_list_cnt].label_length = label_length;

    s_kws_list_cnt++;

    return 0;
}

int LvpInitCtcKws(char *output_file_name, int cpu_count)
{
    g_kws_list.count = s_kws_list_cnt;
    g_kws_list.kws_param_list = g_kws_param_list;

    printf(LOG_TAG"Kws List [Total:%d]:\n", g_kws_list.count);
    for (int i = 0; i < g_kws_list.count; i++) {
        printf(LOG_TAG "KWS: %s | LABEL_LEN: %d | ", g_kws_list.kws_param_list[i].kws_words, g_kws_list.kws_param_list[i].label_length);
        printf("LABEL: [");
        for (int j = 0; j < g_kws_list.kws_param_list[i].label_length; j++) {
            printf("%d", g_kws_list.kws_param_list[i].labels[j]);
            if (j != g_kws_list.kws_param_list[i].label_length - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    for (int i = 0; i < CONFIG_KWS_MODEL_DECODER_WIN_LENGTH; i++) {
        for (int j = 0; j < CONFIG_KWS_MODEL_OUTPUT_LENGTH - 1; j++) {
            s_ctc_decoder_window[i][j] = (1 - 0.9999f) / (CONFIG_KWS_MODEL_OUTPUT_LENGTH - 1);
        }
        s_ctc_decoder_window[i][CONFIG_KWS_MODEL_OUTPUT_LENGTH - 1] = 0.9999f;
    }
    s_ctc_index = 0;

    char output_file_name_with_id[256];
    for (int i = 0; i < cpu_count; i++) {
        sprintf(output_file_name_with_id, "%s%d", output_file_name, i);
        if (output_file_name != NULL) {
            if ((fp_score_output[i] = fopen(output_file_name_with_id, "wb")) == NULL) {
                printf("cannot write file [%s]\n", output_file_name_with_id);
                return -1;
            }
        }
    }

    return 0;
}
