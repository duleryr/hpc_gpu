#include <iostream>
#include <limits>
#include <array>
#include <string>
#include <cstdlib>
#include <unistd.h>

#include "Floyd_Warshall.h"
#include "BlockTimerSeq.h"
#include "BlockTimerPar.h"
#include "Parser.h"
#include "Tools.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))

using namespace std;

int main(int argc, char** argv) 
{
    string filename;
    string category;
    string saveInFile = "n";
    string saveTimes = "n";
    string algorithm;
    int counter = 0;
    int step = 32;
    int numThreads = 4;
    bool chooseGraph = false;

    opterr = 0;
    int c;
    while ((c = getopt (argc, argv, "g:a:dtc:s:n:h")) != -1)
        switch (c)
        {
            case 'a':
                algorithm = optarg;
                if (algorithm != "FW_SEQ" && algorithm != "FW_SEQ_MEM" && algorithm != "FW_SEQ_MEM_SKIP" && algorithm != "FW1_C" && algorithm != "FW1_L" && algorithm != "FW_SEQ" && algorithm != "FW2" && algorithm != "FW_SEQ_TILED" && algorithm != "FW_PAR_TILED" && algorithm != "FW_SEQ_TILED_LAYOUT_MEMPOS" && algorithm != "FW_SEQ_TILED_LAYOUT" && algorithm != "FW_SEQ_TILED_LAYOUT2" && algorithm != "FW_PAR_TILED_LAYOUT" && algorithm != "FW_PAR_TILED_LAYOUT_MEMPOS" && algorithm != "FW_SEQ_1D" && algorithm != "FW_PAR_1D" && algorithm != "FW_GPU") 
                {
                    cerr << "Unknown algorithm : " << algorithm << endl << "List of algorithms : FW_SEQ, FW_SEQ_MEM, FW_SEQ_MEM_SKIP, FW1_C, FW1_L, FW2, FW_SEQ_TILED, FW_PAR_TILED, FW_SEQ_TILED_LAYOUT_MEMPOS, FW_SEQ_TILED_LAYOUT, FW_SEQ_TILED_LAYOUT2, FW_PAR_TILED_LAYOUT, FW_SEQ_1D, FW_PAR_1D, FW_PAR_TILED_LAYOUT_MEMPOS, FW_GPU";
                    return 1;
                }
                break;
            case 'd':
                saveInFile = 'y';
                break;
            case 'n':
                numThreads = atoi(optarg);
                break;
            case 't':
                saveTimes = 'y';
                break;
            case 'g':
                filename = optarg;
                chooseGraph = true;
                break;
            case 'c':
                category = optarg;
                if (category != "simple" && category != "medium" && category != "complex" && category != "very_complex" && category != "huge" && category != "reference") 
                {
                    cerr << "Unknown category : " << category << endl;
                    return 1;
                }
                break;
            case 's':
                step = atoi(optarg);
                break;
            case 'h':
                cout << "Help options :" << endl;
                cout << "-a algorithm : choose one algorithm between {FW_SEQ, FW_SEQ_MEM, FW_SEQ_MEM_SKIP, FW1_C, FW1_L, FW_SEQ_TILED, FW_PAR_TILED, FW_SEQ_TILED_LAYOUT, FW_SEQ_TILED_LAYOUT_MEMPOS, FW_PAR_TILED_LAYOUT, FW_PAR_TILED_LAYOUT_MEMPOS, FW_PAR_1D, FW_GPU, FW2}" << endl;
                cout << "-d : save output distances of each algorithm" << endl;
                cout << "-t : save calcul times of each algorithm" << endl;
                cout << "-c category : apply algorithms to all graph of one category : \"simple\", \"medium\", \"complex\", \"very_complex\", \"huge\"" << endl;
                cout << "-g graph : apply algorithms to the selected graph : \"simple_graphX\", \"medium_graphX\", \"complex_graphX\", \"very_complex_graphX\", \"huge_graphX\"" << endl;
                cout << "-s INT ; choose the step for Floyd_Warshall strip-mined algorithms" << endl;
                cout << "-h : you just did it don't you remember ?" << endl;
                return 0;
            case '?':
                if (optopt == 'c')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                if (optopt == 's')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                if (optopt == 'a')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                if (optopt == 'g')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort ();
        }

    ostream* stream = &cout;
    bool repeat = true;
    while (repeat) 
    {
        if (category == "" && filename == "") {
            cout << "Choose the input file : basic_graphX, simple_graphX, medium_graphX, complex_graphX, very_complex_graphX, huge_graphX" << endl;
            cout << "with X between 0 and 29 : ";
            cin >> filename;
        } else {
            if (!chooseGraph)
                filename = category + "_graph" + to_string(counter);
        }

        vector<vector<unsigned short>> adjacency_matrix = Parser::get_adjacency_matrix("./Input_Files/" + filename + ".txt");
        cout << "----------------------------------------------------" << endl;
        cout << "File : " << filename + ".txt" << endl;

        ofstream ofile("./Calcul_Times/" + filename + ".txt", ios::out);
        if (tolower(saveTimes[0]) == 'y') 
            stream = &ofile;

        int V = adjacency_matrix.size();
        *stream << "V = " << V << ";" << endl;
        vector<vector<unsigned short>> distance_matrix(V, vector<unsigned short>(V));

        /*------------------------------FLOYD_WARSHALL_SEQUENTIAL-----------------------------------*/

        if (algorithm == "" || algorithm == "FW_SEQ") 
        {
            BlockTimerSeq time;
            distance_matrix = floyd_warshall_seq(adjacency_matrix);

            time.display(*stream, "FWSeq", "Floyd_Warshall Sequential", 1);

            if (tolower(saveInFile[0]) == 'y') 
                save_in_file(distance_matrix, filename, "FW_seq");
        }

        /*----------------------------FLOYD_WARSHALL_SEQUENTIAL_MEMOIZATION-----------------------------------*/

        if (algorithm == "" || algorithm == "FW_SEQ_MEM") 
        {
            BlockTimerSeq time;
            distance_matrix = floyd_warshall_seq_mem(adjacency_matrix);

            time.display(*stream, "FWSqM", "Floyd_Warshall Sequential with Memoization", 1);

            if (tolower(saveInFile[0]) == 'y') 
                save_in_file(distance_matrix, filename, "FW_seq_mem");
        }

        /*----------------------------FLOYD_WARSHALL_SEQUENTIAL_MEMOIZATION_SKIP-------------------------------*/

        if (algorithm == "" || algorithm == "FW_SEQ_MEM_SKIP") 
        {
            BlockTimerSeq time;
            distance_matrix = floyd_warshall_seq_mem_skip(adjacency_matrix);

            time.display(*stream, "FWSMS", "Floyd_Warshall Sequential with Memoization and Skipping infinities", 1);

            if (tolower(saveInFile[0]) == 'y') 
                save_in_file(distance_matrix, filename, "FW_seq_mem_skip");
        }

        /*------------------------------FLOYD_WARSHALL_PARALLEL1_C----------------------------------*/
/*           
        if (algorithm == "" || algorithm == "FW1_C")
        {
            step = 20;
            for (int i = 10; i < 40; i++) {
                if ((V % i) == 0)
                    step = i;
            }
            BlockTimerPar time_par;
            distance_matrix = floyd_warshall_par1_c(adjacency_matrix, step);

            time_par.display(*stream, "FWP1C", "Floyd_Warshall Parallel 1D columns", evalNbThreads());

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_par1_c");
        }
*/         
        /*------------------------------FLOYD_WARSHALL_PARALLEL1_L----------------------------------*/
        
        if (algorithm == "" || algorithm == "FW1_L")
        {
            step = 20;
            for (int i = 10; i < 40; i++) {
                if ((V % i) == 0)
                    step = i;
            }
            BlockTimerPar time_par;
            distance_matrix = floyd_warshall_par1_l(adjacency_matrix, step);

            time_par.display(*stream, "FWP1L", "Floyd_Warshall Parallel 1D lines", evalNbThreads());

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_par1_l");
        }
        


        /*------------------------------FLOYD_WARSHALL_SEQUENTIAL_TILED-----------------------------*/
/*
        if (algorithm == "" || algorithm == "FW_SEQ_TILED")
        {
            BlockTimerSeq time;
            distance_matrix = floyd_warshall_seq_tiled(adjacency_matrix, step);

            time.display(*stream, "FWSTI", "Floyd_Warshall Sequential Tiled Implemented", 1);

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_seq_tiled");
        }
*/
        /*------------------------------FLOYD_WARSHALL_PARALLEL_TILED-----------------------------*/
/*
        if (algorithm == "" || algorithm == "FW_PAR_TILED")
        {
            BlockTimerPar time_par;
            distance_matrix = floyd_warshall_par_tiled(adjacency_matrix, step);

            time_par.display(*stream, "FWPTI", "Floyd_Warshall Parallel Tiled Implemented", evalNbThreads());

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_par_tiled");
        }
*/
        /*------------------------------FLOYD_WARSHALL_SEQUENTIAL_TILED_LAYOUT_MEMPOS----------------------*/
/*
        if (algorithm == "" || algorithm == "FW_SEQ_TILED_LAYOUT_MEMPOS")
        {
            step = V;
            for (int i = 2; i < MIN(V, 40); i++) {
                if ((V % i) == 0)
                    step = i;
            }
            BlockTimerSeq time;
            distance_matrix = floyd_warshall_seq_tiled_layout_memPos(adjacency_matrix, step);

            time.display(*stream, "FWSBA", "Floyd_Warshall Sequential Tiled Implemented Cache-Aware, with adapted memory layout stocked in an array", 1);

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_seq_tiled_layout_memPos");
        }
*/
        /*------------------------------FLOYD_WARSHALL_SEQUENTIAL_TILED_LAYOUT----------------------*/

        if (algorithm == "" || algorithm == "FW_SEQ_TILED_LAYOUT")
        {
            int blocSize1D = V;
            for (int i = 2; i < MIN(V, step) + 1; i++) {
                if ((V % i) == 0)
                    blocSize1D = i;
            }
            BlockTimerSeq time;
            distance_matrix = floyd_warshall_seq_tiled_layout(adjacency_matrix, blocSize1D);

            time.display(*stream, "FWSTB", "Floyd_Warshall Sequential Tiled Implemented Cache-Aware, with adapted memory layout", 1);

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_seq_tiled_layout");
        }

        /*------------------------------FLOYD_WARSHALL_SEQUENTIAL_TILED_LAYOUT2----------------------*/
/*
        if (algorithm == "" || algorithm == "FW_SEQ_TILED_LAYOUT2")
        {
            step = 20;
            int blocSize1D = V;
            for (int i = 2; i < MIN(V, step) + 1; i++) {
                if ((V % i) == 0)
                    blocSize1D = i;
            }
            BlockTimerSeq time;
            distance_matrix = floyd_warshall_seq_tiled_layout2(adjacency_matrix, blocSize1D);

            time.display(*stream, "FWST2", "Floyd_Warshall Sequential Tiled Implemented Cache-Aware, with adapted memory layout in reverse", 1);

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_seq_tiled_layout2");
        }
*/
        /*------------------------------FLOYD_WARSHALL_PARALLEL_TILED_LAYOUT_MEMPOS----------------------*/
/*
        if (algorithm == "" || algorithm == "FW_PAR_TILED_LAYOUT_MEMPOS")
        {
            step = V;
            for (int i = 2; i < MIN(V, 160); i++) {
                if ((V % i) == 0)
                    step = i;
            }
            BlockTimerPar time_par;
            distance_matrix = floyd_warshall_par_tiled_layout_memPos(adjacency_matrix, step);

            time_par.display(*stream, "FWPBA", "Floyd_Warshall Parallel Tiled Implemented Cache-Aware, with adapted memory layout stocked in an array", evalNbThreads());

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_par_tiled_layout_memPos");
        }
*/
        /*------------------------------FLOYD_WARSHALL_PARALLEL_TILED_LAYOUT----------------------*/

        if (algorithm == "" || algorithm == "FW_PAR_TILED_LAYOUT")
        {
            int blocSize1D = V;
            for (int i = 2; i < MIN(V, step) + 1; i++) {
                if ((V % i) == 0)
                    blocSize1D = i;
            }
            BlockTimerPar time_par;
            distance_matrix = floyd_warshall_par_tiled_layout(adjacency_matrix, blocSize1D);

            time_par.display(*stream, "FWPTB", "Floyd_Warshall Parallel Tiled Implemented Cache-Aware, with adapted memory layout", evalNbThreads());

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_par_tiled_layout");
        }

        /*------------------------------FLOYD_WARSHALL_SEQUENTIAL_1D----------------------*/

        if (algorithm == "" || algorithm == "FW_SEQ_1D")
        {
            BlockTimerSeq time;
            distance_matrix = floyd_warshall_seq_1d(adjacency_matrix);

            time.display(*stream, "FWS1D", "Floyd_Warshall Sequential with 1d matrix", evalNbThreads());

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_seq_1d");
        }

        /*------------------------------FLOYD_WARSHALL_PARALLEL_1D----------------------*/

        if (algorithm == "" || algorithm == "FW_PAR_1D")
        {
            BlockTimerPar time_par;
            distance_matrix = floyd_warshall_par_1d(adjacency_matrix, numThreads);

            time_par.display(*stream, "FWP1D", "Floyd_Warshall Parallel with 1d matrix", numThreads);

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_par_1d");
        }

        /*------------------------------FLOYD_WARSHALL_GPU----------------------*/

        if (algorithm == "" || algorithm == "FW_GPU")
        {
            BlockTimerPar time_par;
            distance_matrix = floyd_warshall_GPU(adjacency_matrix);

            time_par.display(*stream, "FWGPU", "Floyd_Warshall parallel GPU implemented", 0);

            if (tolower(saveInFile[0]) == 'y')
                save_in_file(distance_matrix, filename, "FW_gpu");
        }

        /*------------------------------FLOYD_WARSHALL_PARALLEL2------------------------------------*/
/*
        if (algorithm == "" || algorithm == "FW2")
        {
            BlockTimerPar time_par;
            distance_matrix = floyd_warshall_par2(adjacency_matrix);

            time_par.display(*stream, "FWP2D", "Floyd_Warshall Parallel 2D", evalNbThreads());

            if (tolower(saveInFile[0]) == 'y') {
                save_in_file(distance_matrix, filename, "FW_par2");
                cout << "Distance matrices saved" << endl;
            }
        }
*/
        /*------------------------------END LOOP----------------------------------------------------*/
        ofile.close();

        if (category == "" && !chooseGraph) {
            cout << "Do you want to do another graph ? (y/N) : " << endl;
            string wantToContinue; cin >> wantToContinue;
            filename = "";
            if (tolower(wantToContinue[0]) != 'y')
                repeat = false;
        } else {
            if (chooseGraph)
                return 0;
            counter++;
            if (counter == 6 && category == "reference") {
                cout << "Generation terminated, exiting..." << endl;
                return 0;
            }
            if (counter == 30) {
                cout << "Generation terminated, exiting..." << endl;
                return 0;
            }
        }
    }
}
