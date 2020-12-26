using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Intrinsics;
using System.Text;

namespace MeshBuilder.Model {
	public class BoundingBox {
		public IEnumerable<Triangle> Triangles { get; }
		public BoundingBox Left { get; }
		public BoundingBox Right { get; }


		public BoundingBox(IEnumerable<Triangle> triangles) {
			Triangles = triangles;
			var enumerable = Triangles.ToList();
			if (enumerable.Count() > 2) (Left, Right) = SplitBoundingBox(this);
			if (Left != null && Right != null) Triangles = null;
		}

		public static (BoundingBox, BoundingBox) SplitBoundingBox(BoundingBox box) {
			var (maxDimension, minDimension) = CalculateDimensions(box.Triangles);
			var rightMaxDimension = new Vertex(maxDimension);
			IEnumerable<Triangle> rightTriangles;
			switch (BiggestDimension(maxDimension, minDimension)) {
				case 0:
					rightMaxDimension.X -= (maxDimension.X - minDimension.X) / 2;
					rightTriangles = box.Triangles.Where(t => (t.V1.X < rightMaxDimension.X
															   && t.V2.X < rightMaxDimension.X)
															  || (t.V1.X < rightMaxDimension.X
																  && t.V3.X < rightMaxDimension.X)
															  || (t.V2.X < rightMaxDimension.X
																  && t.V3.X < rightMaxDimension.X));
					break;
				case 1:
					rightMaxDimension.Y -= (maxDimension.Y - minDimension.Y) / 2;
					rightTriangles = box.Triangles.Where(t => (t.V1.Y < rightMaxDimension.Y
															   && t.V2.Y < rightMaxDimension.Y)
															  || (t.V1.Y < rightMaxDimension.Y
																  && t.V3.Y < rightMaxDimension.Y)
															  || (t.V2.Y < rightMaxDimension.Y
																  && t.V3.Y < rightMaxDimension.Y));
					break;
				case 2:
					rightMaxDimension.Z -= (maxDimension.Z - minDimension.Z) / 2;
					rightTriangles = box.Triangles.Where(t => (t.V1.Z < rightMaxDimension.Z
															   && t.V2.Z < rightMaxDimension.Z)
															  || (t.V1.Z < rightMaxDimension.Z
																  && t.V3.Z < rightMaxDimension.Z)
															  || (t.V2.Z < rightMaxDimension.Z
																  && t.V3.Z < rightMaxDimension.Z));
					break;
				default:
					rightTriangles = box.Triangles.ToList();
					break;
			}

			var enumerable = rightTriangles.ToList();
			return enumerable.Count() == box.Triangles.Count() || !enumerable.Any()
				? (null, null)
				: (new BoundingBox(box.Triangles.Except(enumerable).ToList()), new BoundingBox(enumerable));
		}

		public static int BiggestDimension(Vertex max, Vertex min) {
			var x = max.X - min.X;
			var y = max.Y - min.Y;
			var z = max.Z - min.Z;
			if (x > y && x > z) return 0;
			return y > z ? 1 : 2;
		}

		public static (Vertex, Vertex) CalculateDimensions(IEnumerable<Triangle> triangles) {
			var enumerable = triangles as Triangle[] ?? triangles.ToArray();
			var vertices = enumerable.Select(t => t.V1).Concat(enumerable.Select(t => t.V2))
									 .Concat(enumerable.Select(t => t.V3));
			var enumerable1 = vertices as Vertex[] ?? vertices.ToArray();
			return (new Vertex(enumerable1.Max(v => v.X), enumerable1.Max(v => v.Y), enumerable1.Max(v => v.Z)),
					new Vertex(enumerable1.Min(v => v.X), enumerable1.Min(v => v.Y), enumerable1.Min(v => v.Z)));
		}
	}
}