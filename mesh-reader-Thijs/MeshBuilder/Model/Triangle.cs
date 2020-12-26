namespace MeshBuilder.Model {
	public class Triangle {
		public Vertex V1 { get; }
		public Vertex V2 { get; }
		public Vertex V3 { get; }
		
		public string Indices { get; }

		public Triangle(Vertex v1, Vertex v2, Vertex v3, string i) {
			V1 = v1;
			V2 = v2;
			V3 = v3;
			Indices = i;
		}
	}
}