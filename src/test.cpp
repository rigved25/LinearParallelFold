// 1. multi-loop
{
    for (int p = i-1; p >= std::max(i - SINGLE_MAX_LEN, 0); --p) {
        int nucp = nucs[p];
        int q = next_pair[nucp][j];

        if (use_constraints){
            if (p < i - 1 && !allow_unpaired_position[p+1])
                break;
            if (!allow_unpaired_position[p]){
                q = (*cons)[p];
                if (q < p) break;
            }
            if (q > j+1 && q > allow_unpaired_range[j])
                continue;
            int nucq = nucs[q];
            if (!allow_paired(p, q, cons, nucp, nucq))
                continue;
        }

        if (q != -1 && ((i - p - 1) <= SINGLE_MAX_LEN)) {
            // the current shape is p..i M2 j ..q

            value_type newscore;
#ifdef lv
                newscore = - v_score_multi_unpaired(p+1, i-1) -
                    v_score_multi_unpaired(j+1, q-1) + state.score;
#else
                newscore = score_multi_unpaired(p+1, i-1) +
                    score_multi_unpaired(j+1, q-1) + state.score;
#endif
            update_if_better(bestMulti[q][p], newscore, MANNER_MULTI,
                            static_cast<char>(i - p),
                            q - j);
            #pragma omp atomic
            ++ nos_Multi;
        }
    }
}


// 1. multi-loop: create new bestMulti[q][p]
for (int p = i - 1; p >= std::max(i - SINGLE_MAX_LEN, 0); --p) {
    int nucp = nucs[p];
    int q = next_pair[nucp][j];

    if (use_constraints) {
        if (p < i - 1 && !allow_unpaired_position[p + 1]) break;
        if (!allow_unpaired_position[p]) {
            q = (*cons)[p];
            if (q < p) break;
        }
    }

    while (q != -1 && ((i - p - 1) <= SINGLE_MAX_LEN)) {
        if (use_constraints) {
            if (q > j + 1 && q > allow_unpaired_range[j]) break;
            int nucq = nucs[q];
            if (!allow_paired(p, q, cons, nucp, nucq)) break;
        }

        value_type newscore;
#ifdef lv
        newscore = -v_score_multi_unpaired(p + 1, i - 1) -
                   v_score_multi_unpaired(j + 1, q - 1) + state.score;
#else
        newscore = score_multi_unpaired(p + 1, i - 1) +
                   score_multi_unpaired(j + 1, q - 1) + state.score;
#endif
#pragma omp critical(update_Multi_from_M2)
        update_if_better(bestMulti[q][p], newscore, MANNER_MULTI,
                         static_cast<char>(i - p),
                         q - j);
#pragma omp atomic
        ++nos_Multi;

        q = next_pair[nucp][q];  // keep extending q
    }
}