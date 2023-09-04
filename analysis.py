import sys
import numpy as np
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score

EPS = 0.001
ITER = 200

class Point:
    def __init__(self, coord):
        self.coord = tuple(coord)
        self.dimension = len(self.coord)

    @staticmethod
    def distance(p1, p2):
        #verify dimensions
        if p1.dimension != p2.dimension:
            print("An Error Has Occured")
            sys.exit()

        sum = 0
        for i in range(p1.dimension):
            sum += (p1.coord[i] - p2.coord[i])**2
        
        return sum**0.5

class Cluster:
    def __init__(self, p):
        self.center = p
        self.members = []
    
    def add(self, p):
        self.members.append(p)
    
    def recalc_center(self):
        '''returns eclidean Distance, between the updated centroid to the previous one'''
        coords = [None for i in range(self.center.dimension)]
        for i in range(self.center.dimension):
            sum = 0
            for point in self.members:
                sum += point.coord[i]

            coords[i] = sum / len(self.members)

        new_center = Point(coords)

        delta = abs(Point.distance(new_center, self.center))
        self.center = new_center

        return delta
    
    def __repr__(self):
        st = [f"{comp:.4f}" for comp in self.center.coord]
        return ",".join(st)

    def clear_members(self):
        '''clears any members in the members list'''
        self.members = []


def input_loader(filename):
    # load input file as list of strings
    try:
        with open(filename, 'r') as f:
            lines = f.readlines()
    except:
        print("An Error Has Occurred")
        sys.exit()
    return lines

def lines_to_points(lines):
    '''turns lines into list of points'''
    return [
        Point(
            (float(num)for num in line.split(","))
        ) 
        for line in lines
    ]
    
def kmeans(points, K, iter=ITER, eps=EPS):
    clusters = [Cluster(points[i]) for i in range(K)]
    for i in range(iter):
        for p in points: # step 2
            min_cluster = clusters[0]
            min_dist = Point.distance(p, min_cluster.center)
            
            for cl in clusters:
                curr_dist = Point.distance(p, cl.center)
                if curr_dist < min_dist:
                    min_dist = curr_dist
                    min_cluster = cl
            
            min_cluster.add(p) 

        unchanged_clusters = 0
        for cl in clusters: # step 3
            unchanged_clusters += 1 if cl.recalc_center() < eps else 0
            cl.clear_members()
        if unchanged_clusters == K:
            break
    return clusters       


def print_clusters(clusters):
    for cl in clusters:
        print(cl)

def load_args(args):
    max_iter = ITER
    filename = ""
    if not str.isnumeric(args[1]):
        print("Invalid number of clusters!")
        sys.exit()
    K = int(args[1])

    if args == None or len(args) < 3:
        print("An Error Has Occurred")
        sys.exit()
    elif len(args) == 3:
        filename = args[2]
        
    else:
        if not str.isnumeric(args[2]):
            print("Invalid maximum iteration!")
            sys.exit()
        max_iter = int(args[2])
        filename = args[3]
        check_num_of_iter(max_iter)

    return K, max_iter, filename


def main(args=sys.argv):
    K, max_iter, filename = load_args(args)

    points = lines_to_points(input_loader(filename))
    check_num_of_clusters(K, len(points))

    # Use sklearn's KMeans
    kmeans = KMeans(n_clusters=K, max_iter=max_iter, random_state=0).fit(np.array([p.coord for p in points]))

    # Get cluster assignments and cluster centers
    cluster_assignments = kmeans.labels_
    cluster_centers = kmeans.cluster_centers_

    # Print cluster centers
    for i, center in enumerate(cluster_centers):
        print(f"Cluster {i} Center: {', '.join([f'{comp:.4f}' for comp in center])}")

    # Calculate and print silhouette score
    silhouette_avg = silhouette_score(np.array([p.coord for p in points]), cluster_assignments)
    print(f"kmeans: {silhouette_avg:.4f}")

def check_num_of_clusters(num_of_clusters, num_of_datapoints):
    if num_of_clusters <= 1 or num_of_clusters >= num_of_datapoints:
        print("Invalid number of clusters!")
        sys.exit()

def check_num_of_iter(num_of_iter):
    if num_of_iter <= 1 or num_of_iter >= 1000:
        print("Invalid maximum iteration!")  
        sys.exit()  
    return iter

if __name__=="__main__":
    main()