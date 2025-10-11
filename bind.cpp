#include "Code/Include/basics.h"
#include "Code/Include/desert.h"
#include "Code/Include/vec.h"
#include "pybind11/cast.h"
#include "pybind11/detail/common.h"
#include "pybind11/pytypes.h"
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <tuple>
#include <vector>

namespace py = pybind11;

PYBIND11_MODULE(dune, m) {
  py::class_<DuneSediment>(m, "DuneSediment")
      .def(py::init([](int nx, int ny, double r_min, double r_max,
                       std::tuple<double, double> wind,
                       std::tuple<double, double> cell_size, bool vegetation_on,
                       bool abrasion_on) {
             DuneSediment ds(
                 nx, ny,
                 Box2D(Vector2(0, 0), Vector2(std::get<0>(cell_size) * nx,
                                              std::get<1>(cell_size) * ny)),
                 r_min, r_max, Vector2(std::get<0>(wind), std::get<1>(wind)));
             ds.vegetationOn = vegetation_on;
             ds.abrasionOn = abrasion_on;
             return ds;
           }),
           py::arg("nx"), py::arg("ny"), py::arg("r_min"), py::arg("r_max"),
           py::arg("wind"), py::arg("cell_size") = std::make_tuple(1, 1),
           py::arg("vegetation_on") = false, py::arg("abrasion_on") = false)
      .def_readwrite("nx", &DuneSediment::nx)
      .def_readwrite("ny", &DuneSediment::ny)
      .def_readwrite("vegetation_on", &DuneSediment::vegetationOn)
      .def_readwrite("abrasion_on", &DuneSediment::abrasionOn)
      .def_property_readonly(
          "bedrock",
          [](DuneSediment &ds) {
            using ssize = py::ssize_t;
            std::vector<ssize> shape = {static_cast<ssize>(ds.ny),
                                        static_cast<ssize>(ds.nx)};

            std::vector<ssize> strides = {
                static_cast<ssize>(ds.nx * sizeof(float)),
                static_cast<ssize>(sizeof(float))};

            float *ptr = ds.bedrock.values.data();

            // keep C++ object alive while NumPy array exists
            py::object base = py::cast(&ds);
            return py::array_t<float>(shape, strides, ptr, base);
          })
      .def_property_readonly(
          "sediments",
          [](DuneSediment &ds) {
            using ssize = py::ssize_t;
            std::vector<ssize> shape = {static_cast<ssize>(ds.ny),
                                        static_cast<ssize>(ds.nx)};

            std::vector<ssize> strides = {
                static_cast<ssize>(ds.nx * sizeof(float)),
                static_cast<ssize>(sizeof(float))};

            float *ptr = ds.sediments.values.data();

            // keep C++ object alive while NumPy array exists
            py::object base = py::cast(&ds);
            return py::array_t<float>(shape, strides, ptr, base);
          })
      .def_property_readonly(
          "vegetation",
          [](DuneSediment &ds) {
            using ssize = py::ssize_t;
            std::vector<ssize> shape = {static_cast<ssize>(ds.ny),
                                        static_cast<ssize>(ds.nx)};

            std::vector<ssize> strides = {
                static_cast<ssize>(ds.nx * sizeof(float)),
                static_cast<ssize>(sizeof(float))};

            float *ptr = ds.vegetation.values.data();

            // keep C++ object alive while NumPy array exists
            py::object base = py::cast(&ds);
            return py::array_t<float>(shape, strides, ptr, base);
          })
      .def_property_readonly(
          "wind_x",
          [](DuneSediment &ds) {
            using ssize = py::ssize_t;
            std::vector<ssize> shape = {static_cast<ssize>(ds.ny),
                                        static_cast<ssize>(ds.nx)};

            std::vector<ssize> strides = {
                static_cast<ssize>(ds.nx * sizeof(float)),
                static_cast<ssize>(sizeof(float))};

            float *ptr = ds.windX.values.data();

            // keep C++ object alive while NumPy array exists
            py::object base = py::cast(&ds);
            return py::array_t<float>(shape, strides, ptr, base);
          })
      .def_property_readonly(
          "wind_y",
          [](DuneSediment &ds) {
            using ssize = py::ssize_t;
            std::vector<ssize> shape = {static_cast<ssize>(ds.ny),
                                        static_cast<ssize>(ds.nx)};

            std::vector<ssize> strides = {
                static_cast<ssize>(ds.nx * sizeof(float)),
                static_cast<ssize>(sizeof(float))};

            float *ptr = ds.windY.values.data();

            // keep C++ object alive while NumPy array exists
            py::object base = py::cast(&ds);
            return py::array_t<float>(shape, strides, ptr, base);
          })
      .def_property_readonly(
          "bedrock_hardness",
          [](DuneSediment &ds) {
            using ssize = py::ssize_t;
            std::vector<ssize> shape = {static_cast<ssize>(ds.ny),
                                        static_cast<ssize>(ds.nx)};

            std::vector<ssize> strides = {
                static_cast<ssize>(ds.nx * sizeof(float)),
                static_cast<ssize>(sizeof(float))};

            float *ptr = ds.bedrockHardness.values.data();

            // keep C++ object alive while NumPy array exists
            py::object base = py::cast(&ds);
            return py::array_t<float>(shape, strides, ptr, base);
          })
      .def("step", &DuneSediment::SimulationStepMultiThreadAtomic);
}
