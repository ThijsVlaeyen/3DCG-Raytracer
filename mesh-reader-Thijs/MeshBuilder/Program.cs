using System;
using System.Collections.Generic;
using MeshBuilder.IO;
using MeshBuilder.Model;

namespace MeshBuilder {
    class Program {
        static void Main(string[] args) {
            var bb = MeshReader.ReadMesh("..//..//..//..//dragon.mesh");
            var vertices = MeshReader.ReadVertices("..//..//..//..//dragon.mesh");
            HierarchyWriter.Write(bb, vertices, "..//..//..//..//dragon.bmesh");
        }
    }
}