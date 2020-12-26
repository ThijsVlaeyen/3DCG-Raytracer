using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using MeshBuilder.Model;

namespace MeshBuilder.IO {
	public static class HierarchyWriter {
		public static void Write(BoundingBox box, IEnumerable<Vertex> vertices, string path) {
			using var file = new StreamWriter(path, false);
			
			var enumerable = vertices as Vertex[] ?? vertices.ToArray();
			file.WriteLine(enumerable.Count());
			foreach (var vertex in enumerable) {
				file.WriteLine(vertex.X+" "+vertex.Y+" "+vertex.Z);
			}
			Write(box, file);
			file.Close();
		}

		public static void Write(BoundingBox box, StreamWriter file) {
            if (box.Left == null || box.Right == null) {
				file.WriteLine(box.Triangles.Count());
            	foreach (var triangle in box.Triangles) {
            		file.WriteLine(triangle.Indices);
				}
            } else {
            	Write(box.Left, file);
				Write(box.Right, file);
				file.WriteLine(-1);

			}
		}
	}
}