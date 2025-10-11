#pragma once

#include "basics.h"
#include "noise.h"
#include "vec.h"

class DuneSediment {
private:
  float tanThresholdAngleSediment = 0.60f;      // ~33°
  float tanThresholdAngleWindShadowMin = 0.08f; // ~5°
  float tanThresholdAngleWindShadowMax = 0.26f; // ~15°
  float tanThresholdAngleBedrock = 2.5f;        // ~68°

protected:
  Box2D box;          //!< World space bounding box.
  float matterToMove; //!< Amount of sand transported by the wind, in meter.
  float cellSize; //!< Size of one cell in meter, squared. Stored to speed up
                  //!< the simulation.

public:
  int nx, ny; //!< Grid resolution.
  bool vegetationOn = false;
  bool abrasionOn = false;

  ScalarField2D bedrock;    //!< Bedrock elevation layer, in meter.
  ScalarField2D sediments;  //!< Sediment elevation layer, in meter.
  ScalarField2D vegetation; //!< Vegetation presence in [0, 1].
  ScalarField2D windX;
  ScalarField2D windY;
  ScalarField2D bedrockHardness;

  DuneSediment(int nx, int ny);
  DuneSediment(int nx, int ny, const Box2D &bbox, float rMin, float rMax,
               const Vector2 &w);
  ~DuneSediment();

  // Simulation
  int ToIndex1D(const Vector2i &q) const;
  int ToIndex1D(int i, int j) const;
  void SimulationStepMultiThreadAtomic();
  void EndSimulationStep();
  void SimulationStepWorldSpace();
  void PerformReptationOnCell(int i, int j, int bounce);
  void ComputeWindAtCell(int i, int j, Vector2 &windDir) const;
  float IsInShadow(int i, int j, const Vector2 &wind) const;
  void SnapWorld(Vector2 &p) const;
  int CheckSedimentFlowRelative(const Vector2i &p, float tanThresholdAngle,
                                Vector2i *nei, float *nslope) const;
  int CheckBedrockFlowRelative(const Vector2i &p, float tanThresholdAngle,
                               Vector2i *nei, float *nslope) const;
  void StabilizeSedimentRelative(int i, int j);
  bool StabilizeBedrockRelative(int i, int j);
  void StabilizeBedrockAll();
  void PerformAbrasionOnCell(int i, int j, const Vector2 &windDir);

  // Exports
  void ExportObj(const std::string &file) const;
  void ExportJPG(const std::string &url) const;

  // Inlined functions and query
  float Height(int i, int j) const;
  float Height(const Vector2 &p) const;
  float Bedrock(int i, int j) const;
  float Sediment(int i, int j) const;
  void SetAbrasionMode(bool c);
  void SetVegetationMode(bool c);
};

/*!
\brief Compute the 1D index from a given grid vertex.
\param q grid vertex.
*/
inline int DuneSediment::ToIndex1D(int i, int j) const {
  return bedrock.ToIndex1D(i, j);
}

/*!
\brief Compute the 1D index from a given grid vertex.
\param q grid vertex.
*/
inline int DuneSediment::ToIndex1D(const Vector2i &q) const {
  return bedrock.ToIndex1D(q);
}

/*!
\brief
*/
inline float DuneSediment::Height(int i, int j) const {
  return bedrock.Get(i, j) + sediments.Get(i, j);
}

/*!
\brief
*/
inline float DuneSediment::Height(const Vector2 &p) const {
  return bedrock.GetValueBilinear(p) + sediments.GetValueBilinear(p);
}

/*!
\brief
*/
inline float DuneSediment::Bedrock(int i, int j) const {
  return bedrock.Get(i, j);
}

/*!
\brief
*/
inline float DuneSediment::Sediment(int i, int j) const {
  return sediments.Get(i, j);
}

/*!
\brief
*/
inline void DuneSediment::SetAbrasionMode(bool c) {
  abrasionOn = c;
  if (!abrasionOn) {
    return;
  }

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      const float freq = 0.08f;
      const float warp = 15.36f;
      const Vector2 p = bedrock.ArrayVertex(i, j);

      // Bedrock resistance [0, 1] (1.0 equals to weak, 0.0 equals to hard)
      // Here with a simple sin() function, but anything could be used: texture,
      // noise, construction trees... In the paper, we used various noises
      // octaves combined with each other. Note: To get a more interesting look
      // on the yardangs, turbulent wind is required. It is not provided In this
      // implementation.

      float h = sinf((p.y * freq) + (warp * PerlinNoise::GetValue(0.05f * p)));
      h = (h + 1) / 2.0f;
      bedrockHardness.Set(i, j, h);
    }
  }
}

/*!
\brief
*/
inline void DuneSediment::SetVegetationMode(bool c) {
  vegetationOn = c;
  if (!vegetationOn) {
    return;
  }

  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      // Vegetation
      // Arbitrary clamped 2D noise - but you can use whatever you want.
      Vector3 vector = Vector3(i * 7.91247f, j * 7.91247f, 0.0f);
      float v = PerlinNoise::fBm(vector, 1.0f, 0.002f, 3);
      v = v / 1.75f;

      if (v > 0.45f)
        vegetation.Set(i, j, 0.85f);
    }
  }
}
