--[[ @doc
# Reading a 2D .obj file and extrude it

## Reading the Mesh
We start by reading a 2D obj file that we will extrude. (See a previous tutorial for reading 2D obj file). We inserts
2 layers between z=0 and z=1.1, followed by 3 layers, between z=1.1 and z=2.1.

Finally, we export the mesh to a VTU file.

The resulting mesh and material layout is shown below:

![Mesh_Material](images/extruded_mesh_material.png)

The resulting mesh and partition is shown below:

![Mesh_Partition](images/extruded_mesh_partition.png)

--]]
-- Setup the mesh
meshgen = mesh.ExtruderMeshGenerator.Create({
  inputs = {
    mesh.FromFileMeshGenerator.Create({
      filename = "./tri_2mat_bc_1542.obj",
    }),
  },
  layers = { { z = 1.1, n = 2 }, { z = 2.1, n = 3 } },
  partitioner = mesh.PETScGraphPartitioner.Create({ type = "parmetis" }),
})
grid = meshgen:Execute()

mesh.ExportToPVTU(grid, "Extruded_mesh_only")
