set -e

for benchmark in relu sigmoid tanh gelu softmax; do
# for benchmark in relutruncate; do
  ./build/micro-$benchmark 2 &> /dev/null
  OMP_NUM_THREADS=4 ./build/micro-$benchmark $@ &> tmp.txt
  echo "Benchmarking $benchmark"
  echo "======================="
  cat tmp.txt
  echo ""
  rm tmp.txt
done
