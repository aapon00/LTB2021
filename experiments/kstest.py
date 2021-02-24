from scipy import stats
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

in_files = [
    "/zfs/dicelab/DigitalPlant/tcpdump.raw.pareto.2.47250.csv"
#    "pareto_distribution/shape_1/median_target/pareto_1000_median_shape_1.58840.csv",
#   "pareto_distribution/shape_1/median_target/pareto_500_median_shape_1.58838.csv",
#   "pareto_distribution/shape_1/median_target/pareto_100_median_shape_1.58832.csv",
#   "pareto_distribution/shape_1/median_target/pareto_50_median_shape_1.58830.csv",
#    "pareto_distribution/shape_2/median_target/pareto_1000_median_shape_2.58854.csv",
#    "pareto_distribution/shape_2/median_target/pareto_500_median_shape_2.58852.csv",
#    "pareto_distribution/shape_2/median_target/pareto_100_median_shape_2.58848.csv",
#    "pareto_distribution/shape_2/median_target/pareto_50_median_shape_2.58846.csv",
#    "pareto_distribution/shape_3/median_target/pareto_1000_median_shape_3.58866.csv",
#    "pareto_distribution/shape_3/median_target/pareto_500_median_shape_3.58864.csv",
#    "pareto_distribution/shape_3/median_target/pareto_100_median_shape_3.58862.csv",
#    "pareto_distribution/shape_3/median_target/pareto_50_median_shape_3.58858.csv"
]
out_file = "cloud_validation.csv"
shapes = [
    2.0
#   1.0,
#   1.0,
#   1.0,
#   1.0,
#   2.0,
#   2.0,
#   2.0,
#   2.0,
#   3.0,
#   3.0,
#   3.0,
#   3.0
]
scales = [
    0.0707
#    ?,
#   0.250,
#   0.050,
#   0.025,
#   0.7071,
#   0.3536,
#   0.0707,
#   0.0354,
#   0.7937,
#   0.3969,
#   0.0794,
#   0.0397
]

# @require scale, shape > 0
def pdf(x, scale, shape):
    if x < scale:
        return 0
    else:
        val = (shape * scale ** shape) / (x ** (shape + 1))
        return val

def pareto_mean(scale, shape):
    if shape <= 1:
        return 999999999
    else:
        return shape * scale / (shape - 1)

def pareto_median(scale, shape):
    return scale * 2 ** (1/shape)

def calc_mean(arr):
    return np.sum(arr) / len(arr)

def calc_median(arr):
    np.sort(arr)
    length = len(arr)
    if (length % 2) != 0:
        return arr[int(length/2)]
    else:
        return (arr[int(length/2)] + arr[int(length/2 - 1)]) / 2

def get_statistics():
    out_df = pd.DataFrame(columns=["file", "mean", "median", "pareto mean", "pareto median", "kstest d", "kstest p-value"])
    for i in range(len(in_files)):
        df = pd.read_csv(in_files[i])

       
        df = df[50:-50] # drop the first and last 50 entries

        csv_array = df.to_numpy()

        gaps = csv_array[:, 3]
        #gaps = gaps * 1000
        unique_gaps = np.unique(gaps)

        sorted_gaps = np.sort(unique_gaps)

        rvs = stats.pareto.rvs(shapes[i], scale=scales[i], size=len(sorted_gaps))
        rvs = np.sort(rvs)

        norm_rvs = stats.norm.rvs(size=len(sorted_gaps))
        norm_rvs = np.sort(norm_rvs)

        # used as y axis for CDF
        # p = np.arange(len(unique_gaps)) / (len(unique_gaps) - 1)

        # plt.plot(sorted_gaps, p)
        # plt.xlabel("gaps")
        # plt.ylabel("P(X<=x)")
        # plt.show()

        # used as y axis for PDF
        fig, ax = plt.subplots(1,1)
        f_x = []
        f_x_rvs = []
        f_x_norm = []
        for j in range(len(sorted_gaps)):
            f_x.append(pdf(sorted_gaps[j], scales[i], shapes[i]))
            f_x_rvs.append(pdf(rvs[j], scales[i], shapes[i]))
            f_x_norm.append(pdf(norm_rvs[j], scales[i], shapes[i]))


        #ax.plot(sorted_gaps, f_x)
        #ax.plot(rvs, f_x_rvs)
        #ax.plot(norm_rvs, f_x_norm)
        # plt.xlabel("gaps")
        # plt.ylabel("P(X=x)?")
        # plt.show()

        mean = calc_mean(sorted_gaps)
        median = calc_median(sorted_gaps)
        p_mean = pareto_mean(scales[i], shapes[i])
        p_median = pareto_median(scales[i], shapes[i])
        kstest_d, kstest_p = stats.kstest(sorted_gaps.tolist(), "pareto", (shapes[i], 0, scales[i]))

        stat_dict = {"file": in_files[i], "mean": mean, "median": median, "pareto mean": p_mean, "pareto median": p_median, "kstest d": kstest_d, "kstest p-value": kstest_p}
        out_df = out_df.append(stat_dict, ignore_index=True)
    return out_df

def main():
    df = get_statistics()
    df.to_csv(out_file, index=False)

main()
