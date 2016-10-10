/* CS/COE 1541
	just compile with gcc -o pipeline pipeline.c
   just compile with gcc -o pipeline pipeline.c
   and execute using
   ./pipeline  /afs/cs.pitt.edu/courses/1541/short_traces/sample.tr	0
***************************************************************/

#include <stdio.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include "CPU.h"

int main(int argc, char **argv)
{
    struct trace_item *tr_entry;
    size_t size;
    char *trace_file_name;
    int trace_view_on = 0;
    int prediction_method = 0;
    int hazard = 0;
    int prediction_array_size = 64;
    int branch_hazard = 0;

    int prediction_values[prediction_array_size];
    int prediction_pc[prediction_array_size];
    for(int i = 0; i < prediction_array_size; i++)
    {
        prediction_values[i] = 0;
        prediction_pc[i] = 2;
    }

    struct trace_item no_op;
    no_op.type = ti_NOP;
    no_op.sReg_a = 255;
    no_op.sReg_b = 255;
    no_op.dReg = 255;
    no_op.PC = 0;
    no_op.Addr = 0;

    struct trace_item end_op;
    end_op.type = 9;
    end_op.sReg_a = 255;
    end_op.sReg_b = 255;
    end_op.dReg = 255;
    end_op.PC = 0;
    end_op.Addr = 0;

    struct trace_item squashed;
    squashed.type = 10;
    squashed.sReg_a = 255;
    squashed.sReg_b = 255;
    squashed.dReg = 255;
    squashed.PC = 0;
    squashed.Addr = 0;

    /*
            This is the buffer array
            pipeline[0] = IF1
            pipeline[1] = IF2
            pipeline[2] = ID
            pipeline[3] = EX1
            pipeline[4] = EX2
            pipeline[5] = MEM1
            pipeline[6] = MEM2
            pipeline[7] = WB
    */
    struct trace_item *pipeline[8];
    for(int i = 0; i < 8; i++)
    {
        pipeline[i] = &no_op;
    }

    int branch_taken[5];
    for(int i = 0; i< 5; i++)
    {
        branch_taken[i] = 0;
    }

    unsigned char t_type = 0;
    unsigned char t_sReg_a= 0;
    unsigned char t_sReg_b= 0;
    unsigned char t_dReg= 0;
    unsigned int t_PC = 0;
    unsigned int t_Addr = 0;

    unsigned int cycle_number = 0;

    if (argc == 1) {
        fprintf(stdout, "\nUSAGE: tv <trace_file> <switch - any character>\n");
        fprintf(stdout, "\n(switch) to turn on or off individual item view.\n\n");
        exit(0);
    }

    trace_file_name = argv[1];
    if (argc >= 3) prediction_method = atoi(argv[2]);
    if (argc == 4) trace_view_on = atoi(argv[3]);

    fprintf(stdout, "\n ** opening file %s\n", trace_file_name);

    trace_fd = fopen(trace_file_name, "rb");

    if (!trace_fd) {
        fprintf(stdout, "\ntrace file %s not opened.\n\n", trace_file_name);
        exit(0);
    }

    trace_init();

    while(1)
    {
        //you are in branch loop and adding conditions.
        if(branch_hazard != 0)
        {
            pipeline[7] = pipeline[6];
            pipeline[6] = pipeline[5];
            pipeline[5] = pipeline[4];
            pipeline[4] = &squashed;
            hazard = 1;
            branch_hazard--;
        }
        //branch taken and prediction method is 0
        else if(prediction_method == 0 && pipeline[4]->type == ti_BRANCH && branch_taken[4]==1)
        {
            pipeline[7] = pipeline[6];
            pipeline[6] = pipeline[5];
            pipeline[5] = pipeline[4];
            pipeline[4] = &squashed;
            hazard = 1;
            branch_hazard = 3;
        }
        //branch taken and prediction method is 1+
        else if(prediction_method != 0 && pipeline[4]->type == ti_BRANCH && branch_taken[4] == 1)
        {
            int pc_hash = ((pipeline[4]->PC)>>4)&(prediction_array_size-1);
            //pc appears in prediction table.
            if(prediction_pc[pc_hash] == pipeline[4]->PC && (prediction_values[pc_hash] == 0 || prediction_values[pc_hash] == 1))
            {
                pipeline[7] = pipeline[6];
                pipeline[6] = pipeline[5];
                pipeline[5] = pipeline[4];
                pipeline[4] = &squashed;
                hazard = 1;
                branch_hazard = 3;
            }
            if(prediction_pc[pc_hash] != pipeline[4]->PC)
            {
                pipeline[7] = pipeline[6];
                pipeline[6] = pipeline[5];
                pipeline[5] = pipeline[4];
                pipeline[4] = &squashed;
                hazard = 1;
                branch_hazard = 3;
                prediction_values[pc_hash] = 0;
            }

            prediction_pc[pc_hash] = pipeline[4]->PC;
            if(prediction_method ==1)
            {
                prediction_values[pc_hash] = 3;
            }
            else
            {
                switch(prediction_pc[pc_hash])
                {
                    case 0:
                        prediction_values[pc_hash] = 1;
                        break;
                    case 1:
                    case 2:
                    case 3:
                        prediction_values[pc_hash] = 3;
                        break;
                }
            }
        }
        //branch not taken and prediction method is 1+
        else if(prediction_method != 0 && pipeline[4]->type == ti_BRANCH)
        {
            int pc_hash = ((pipeline[4]->PC)>>4)&(prediction_array_size-1);
            //pc appears in prediction table
            if(prediction_pc[pc_hash] == pipeline[4]->PC && (prediction_values[pc_hash] == 2 || prediction_values[pc_hash] == 3))
            {
                pipeline[7] = pipeline[6];
                pipeline[6] = pipeline[5];
                pipeline[5] = pipeline[4];
                pipeline[4] = &squashed;
                hazard = 1;
                branch_hazard = 3;
            }
            if(prediction_pc[pc_hash] != pipeline[4]->PC)
            {
                prediction_values[pc_hash] = 0;
            }

            prediction_pc[pc_hash] = pipeline[4]->PC;
            if(prediction_method ==1)
            {
                prediction_values[pc_hash] = 0;
            }
            else
            {
                switch(prediction_pc[pc_hash])
                {
                    case 0:
                    case 1:
                    case 2:
                        prediction_values[pc_hash] = 0;
                        break;
                    case 3:
                        prediction_values[pc_hash] = 1;
                        break;
                }
            }
        }
        //data hazards
        else if(
            ((pipeline[3]->dReg == pipeline[2]->sReg_a || pipeline[3]->dReg == pipeline[2]->sReg_b) && pipeline[3]->dReg != 255) ||
            (pipeline[4]->type == ti_LOAD && (pipeline[4]->dReg == pipeline[2]->sReg_a || pipeline[4]->dReg == pipeline[2]->sReg_b) && pipeline[4]->dReg !=255) ||
            (pipeline[5]->type == ti_LOAD && (pipeline[5]->dReg == pipeline[2]->sReg_a || pipeline[5]->dReg == pipeline[2]->sReg_b) && pipeline[5]->dReg !=255)
        )
        {
            pipeline[7] = pipeline[6];
            pipeline[6] = pipeline[5];
            pipeline[5] = pipeline[4];
            pipeline[4] = pipeline[3];
            pipeline[3] = &no_op;
            hazard = 1;
        }
        //structural hazards
        else if((pipeline[7]->dReg == pipeline[2]->sReg_a || pipeline[7]->dReg == pipeline[2]->sReg_b)&&pipeline[7]->dReg!=255)
        {
            pipeline[7] = pipeline[6];
            pipeline[6] = pipeline[5];
            pipeline[5] = pipeline[4];
            pipeline[4] = pipeline[3];
            pipeline[3] = &no_op;
            hazard = 1;
        }

        if(hazard == 0)
        {
            pipeline[7] = pipeline[6];
            pipeline[6] = pipeline[5];
            pipeline[5] = pipeline[4];
            pipeline[4] = pipeline[3];
            pipeline[3] = pipeline[2];
            pipeline[2] = pipeline[1];
            pipeline[1] = pipeline[0];
            size = trace_get_item(&pipeline[0]);
            if(!size)
            {
                pipeline[0] = &end_op;
            }
        }
        hazard = 0;

        if(pipeline[1]->type == ti_BRANCH && pipeline[1]->Addr == pipeline[0]->PC)
        {
            branch_taken[4] = branch_taken[3];
            branch_taken[3] = branch_taken[2];
            branch_taken[2] = branch_taken[1];
            branch_taken[1] = 1;
        }
        else
        {
            branch_taken[4] = branch_taken[3];
            branch_taken[3] = branch_taken[2];
            branch_taken[2] = branch_taken[1];
            branch_taken[1] = branch_taken[0];
        }

        if(pipeline[7]->type == 9)
        {
            printf("+ Simulation terminates at cycle : %u\n", cycle_number);
            break;
        }

        cycle_number++;

        if (trace_view_on) /* print the executed instruction if trace_view_on=1 */
        {
            tr_entry = pipeline[7];
            t_type = tr_entry->type;
            t_sReg_a = tr_entry->sReg_a;
            t_sReg_b = tr_entry->sReg_b;
            t_dReg = tr_entry->dReg;
            t_PC = tr_entry->PC;
            t_Addr = tr_entry->Addr;
            switch(tr_entry->type)
            {
                case ti_NOP:
                    printf("[cycle %d] NOP:\n",cycle_number) ;
                    break;
                case ti_RTYPE:
                    printf("[cycle %d] RTYPE:",cycle_number) ;
                    printf(" (PC: %x)(sReg_a: %d)(sReg_b: %d)(dReg: %d) \n", tr_entry->PC, tr_entry->sReg_a, tr_entry->sReg_b, tr_entry->dReg);
                    break;
                case ti_ITYPE:
                    printf("[cycle %d] ITYPE:",cycle_number) ;
                    printf(" (PC: %x)(sReg_a: %d)(dReg: %d)(addr: %x)\n", tr_entry->PC, tr_entry->sReg_a, tr_entry->dReg, tr_entry->Addr);
                    break;
                case ti_LOAD:
                    printf("[cycle %d] LOAD:",cycle_number) ;
                    printf(" (PC: %x)(sReg_a: %d)(dReg: %d)(addr: %x)\n", tr_entry->PC, tr_entry->sReg_a, tr_entry->dReg, tr_entry->Addr);
                    break;
                case ti_STORE:
                    printf("[cycle %d] STORE:",cycle_number) ;
                    printf(" (PC: %x)(sReg_a: %d)(sReg_b: %d)(addr: %x)\n", tr_entry->PC, tr_entry->sReg_a, tr_entry->sReg_b, tr_entry->Addr);
                    break;
                case ti_BRANCH:
                    printf("[cycle %d] BRANCH:",cycle_number) ;
                    printf(" (PC: %x)(sReg_a: %d)(sReg_b: %d)(addr: %x)\n", tr_entry->PC, tr_entry->sReg_a, tr_entry->sReg_b, tr_entry->Addr);
                    break;
                case ti_JTYPE:
                    printf("[cycle %d] JTYPE:",cycle_number) ;
                    printf(" (PC: %x)(addr: %x)\n", tr_entry->PC,tr_entry->Addr);
                    break;
                case ti_SPECIAL:
                    printf("[cycle %d] SPECIAL:",cycle_number) ;
                    break;
                case ti_JRTYPE:
                    printf("[cycle %d] JRTYPE:",cycle_number) ;
                    printf(" (PC: %x) (sReg_a: %d)(addr: %x)\n", tr_entry->PC, tr_entry->dReg, tr_entry->Addr);
                    break;
                case 10:
                    printf("[cycle %d] SQUASHED:\n",cycle_number);

            }
        }
    }


    trace_uninit();

    exit(0);

}
