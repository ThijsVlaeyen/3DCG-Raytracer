namespace MeshBuilder.Model {
	public class Vertex {
		public double X { get; set; }
		public double Y { get; set; }
		public double Z { get; set; }

		public Vertex(double x, double y, double z) {
			X = x;
			Y = y;
			Z = z;
		}

		public Vertex(Vertex v) {
			X = v.X;
			Y = v.Y;
			Z = v.Z;
		}
	}
}