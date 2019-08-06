#include <iostream>
#include <cstdint>
#include <unistd.h>
#include <ppm.h>

const uint64_t NumberOfCluster = 8;
const std::string DefaultInputFileName  = "input.ppm";
const std::string DefaultOutputFileName = "output.ppm";

struct KMeansItem
{
  std::vector<uint64_t> cluster_table;
  std::vector<RGBColor> color_mean;
  
  KMeansItem(uint64_t width, uint64_t height, uint64_t num_of_cluster)
  {
    cluster_table = std::vector<uint64_t>(width * height, 0);
    color_mean = std::vector<RGBColor>(num_of_cluster);
  }
  
  bool operator==(const KMeansItem &rhs) const
  {
    return cluster_table == rhs.cluster_table;
  }
};

int k_means_calc_mean(KMeansItem &item, const PPM &ppm_image, uint64_t NumberOfCluster)
{
  std::vector<uint64_t> counter(NumberOfCluster, 0);
  
  for (uint64_t y = 0; y < ppm_image.height(); ++y) {
    for (uint64_t x = 0; x < ppm_image.width(); ++x) {
      uint64_t index = y * ppm_image.width() + x;
      uint64_t cluster = item.cluster_table[index];
      
      item.color_mean[cluster] += ppm_image.get(x, y);
      ++counter[cluster];
    }
  }
  
  for (uint64_t cluster = 0; cluster < NumberOfCluster; ++cluster) {
    if (counter[cluster] > 0) {
      item.color_mean[cluster] /= counter[cluster];
    }
  }
  
  return 0;
}

KMeansItem k_means_init(const PPM &ppm_image, uint64_t NumberOfCluster)
{
  KMeansItem item(ppm_image.width(), ppm_image.height(), NumberOfCluster);
  
  for (uint64_t y = 0; y < ppm_image.height(); ++y) {
    for (uint64_t x = 0; x < ppm_image.width(); ++x) {
      uint64_t index = y * ppm_image.width() + x;
      uint64_t cluster = index % NumberOfCluster;
      item.cluster_table[index] = cluster;
    }
  }
  
  k_means_calc_mean(item, ppm_image, NumberOfCluster);
  
  return item;
}

KMeansItem k_means_step(const KMeansItem &item, const PPM &ppm_image, uint64_t NumberOfCluster)
{
  KMeansItem new_item(ppm_image.width(), ppm_image.height(), NumberOfCluster);
  
  for (uint64_t y = 0; y < ppm_image.height(); ++y) {
    for (uint64_t x = 0; x < ppm_image.width(); ++x) {
      uint64_t index = y * ppm_image.width() + x;
      
      RGBColor color = ppm_image.get(x, y);
      uint64_t min = UINT64_MAX, min_cluster = -1;
      for (uint64_t cluster = 0; cluster < NumberOfCluster; ++cluster) {
        uint64_t dist = color.dist(item.color_mean[cluster]);
        if (min > dist) {
          min = dist;
          min_cluster = cluster;
        }
      }
      new_item.cluster_table[index] = min_cluster;
    }
  }
  
  k_means_calc_mean(new_item, ppm_image, NumberOfCluster);
  
  return new_item;
}

int convert(uint64_t num_of_cluster, const std::string &input_file, const std::string &output_file)
{
  PPM ppm_image;
  
  int ifs_ret = ppm_image.read_file(input_file.c_str());
  if (ifs_ret) {
    std::cerr << "Cannot open file " << input_file << std::endl;
    return 1;
  }

  KMeansItem item = k_means_init(ppm_image, num_of_cluster);
  
  while (true) {
    KMeansItem new_item = k_means_step(item, ppm_image, num_of_cluster);
    
    if (new_item == item) { break; }
    
    item = new_item;
  }
  
  for (uint64_t y = 0; y < ppm_image.height(); ++y) {
    for (uint64_t x = 0; x < ppm_image.width(); ++x) {
      uint64_t index = y * ppm_image.width() + x;
      uint64_t cluster = item.cluster_table[index];
      ppm_image.set(x, y, item.color_mean[cluster]);
    }
  }
  
  int ofs_ret = ppm_image.write_file(output_file.c_str());
  if (ofs_ret) {
    std::cerr << "Cannot open file " << output_file << std::endl;
    return 1;
  }
  return 0;
}

void usage(const char *prg_name)
{
  std::cerr << "[Usage] " << prg_name << " [ -n <number of clusters> ] [ -i <input file> ] [ -o <output file> ]"  << std::endl;
  std::cerr << "  -h                      : show usage" << std::endl;
  std::cerr << "  -n <number of clusters> : set number of cluster ; default = " << NumberOfCluster << std::endl;
  std::cerr << "  -i <input file>         : set input file name   ; default = " << DefaultInputFileName << std::endl;
  std::cerr << "  -o <output file>        : set output file name  ; default = " << DefaultOutputFileName << std::endl;
}

int main(int argc, char **argv)
{
  uint64_t num_of_cluster = NumberOfCluster;
  std::string input_file = DefaultInputFileName, output_file = DefaultOutputFileName;

  int opt;
  while ((opt = getopt(argc, argv, "hi:n:o:")) != -1) {
    switch(static_cast<char>(opt)) {
    case 'h': usage(argv[0]); exit(0);
    case 'n': num_of_cluster = atoi(optarg); break;
    case 'i': input_file = std::string(optarg); break;
    case 'o': output_file = std::string(optarg); break;
    default: break;
    }
  }

  convert(num_of_cluster, input_file, output_file);

  return 0;
}
