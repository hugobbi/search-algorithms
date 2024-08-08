BEGIN {
    FS=" "  # Set the field separator to space
    print "k,avg_search_time_diff,avg_total_time_diff,avg_expanded_diff"
}

{
    if ($1 == "instance") {
        instance = $2
        k = $4
    } else if ($1 == "Built-in") {
        implementation = "Built-in"
    } else if ($1 == "PlanOpt") {
        implementation = "PlanOpt"
    } else if ($1 == "Expanded" && $2 == "until" && $3 == "last" && $4 == "jump:") {
        expanded = $5
        if (implementation == "Built-in") {
            built_in_expanded[instance, k] = expanded
        } else if (implementation == "PlanOpt") {
            planopt_expanded[instance, k] = expanded
        }
    } else if ($1 == "Search") {
        search_time = $3
        if (implementation == "Built-in") {
            built_in_search_time[instance, k] = search_time
        } else if (implementation == "PlanOpt") {
            planopt_search_time[instance, k] = search_time
        }
    } else if ($1 == "Total") {
        total_time = $3
        if (implementation == "Built-in") {
            built_in_total_time[instance, k] = total_time
        } else if (implementation == "PlanOpt") {
            planopt_total_time[instance, k] = total_time
        }
    }
}

END {
    for (key in built_in_search_time) {
        split(key, keys, SUBSEP)
        instance = keys[1]
        k = keys[2]
        search_time_diff = planopt_search_time[instance, k] - built_in_search_time[instance, k]
        total_time_diff = planopt_total_time[instance, k] - built_in_total_time[instance, k]
        expanded_diff = planopt_expanded[instance, k] - built_in_expanded[instance, k]
        search_time_diff_sum[k] += search_time_diff
        total_time_diff_sum[k] += total_time_diff
        expanded_diff_sum[k] += expanded_diff
        count[k]++
    }

    for (k in count) {
        avg_search_time_diff = search_time_diff_sum[k] / count[k]
        avg_total_time_diff = total_time_diff_sum[k] / count[k]
        avg_expanded_diff = expanded_diff_sum[k] / count[k]
        print k "," avg_search_time_diff "," avg_total_time_diff "," avg_expanded_diff
    }
}
