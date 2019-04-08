#include <iostream>
#include <cstdint>
#include <ppm.h>

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

int main(int argc, char **argv)
{
  const uint64_t NumberOfCluster = 2;
  
  PPM ppm_image;
  
  int ifs_ret = ppm_image.read_file("./input.ppm");
  if (ifs_ret) {
    std::cerr << "Cannot open file ./input.ppm" << std::endl;
    return 1;
  }

  KMeansItem item = k_means_init(ppm_image, NumberOfCluster);
  
  while (true) {
    KMeansItem new_item = k_means_step(item, ppm_image, NumberOfCluster);
    
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
  
  int ofs_ret = ppm_image.write_file("./output.ppm");
  if (ofs_ret) {
    std::cerr << "Cannot open file ./output.ppm" << std::endl;
    return 1;
  }

  return 0;
}
