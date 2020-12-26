using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using MeshBuilder.Model;

namespace MeshBuilder.IO {
	public static class MeshReader {
		public static BoundingBox ReadMesh(string path) {
			using var file = new StreamReader(path);
			var nrOfVertices = int.Parse(file.ReadLine() ?? throw new ArgumentException("Wrong file"));
			var vertices = new List<Vertex>();
			for (var i = 0; i < nrOfVertices; i++) {
				var ln = file.ReadLine();
				if (ln == null) continue;
				var vector = ln.Split(" ");
				vertices.Add(new Vertex(double.Parse(vector[0]), double.Parse(vector[1]), double.Parse(vector[2])));
			}
			var nrOfTriangles = int.Parse(file.ReadLine() ?? throw new ArgumentException("Wrong file"));
			var triangles = new List<Triangle>();
			for (var i = 0; i < nrOfTriangles; i++) {
				var ln = file.ReadLine();
				if (ln == null) continue;
				var vector = ln.Split(" ");
				triangles.Add(new Triangle(vertices[int.Parse(vector[0])], vertices[int.Parse(vector[1])], vertices[int.Parse(vector[2])], ln));
			}
			file.Close();
			return new BoundingBox(triangles);
		}

		public static IEnumerable<Vertex> ReadVertices(string path) {
			using var file = new StreamReader(path);
			var vertices = ReadVertices(file);
			file.Close();
			return vertices;
		}

		public static IEnumerable<Vertex> ReadVertices(StreamReader file) {
			var nrOfVertices = int.Parse(file.ReadLine() ?? throw new ArgumentException("Wrong file"));
			var vertices = new List<Vertex>();
			for (var i = 0; i < nrOfVertices; i++) {
				var ln = file.ReadLine();
				if (ln == null) continue;
				var vector = ln.Split(" ");
				vertices.Add(new Vertex(double.Parse(vector[0]), double.Parse(vector[1]), double.Parse(vector[2])));
			}

			return vertices;
		}
	}
}