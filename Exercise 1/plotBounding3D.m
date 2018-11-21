function plotBounding3D(vertex_coord,img)

    figure
    imshow(img)
    hold on
    plot(vertex_coord(1,[1,2]),vertex_coord(2,[1,2]),'b','LineWidth',3)
    plot(vertex_coord(1,[2,3]),vertex_coord(2,[2,3]),'b','LineWidth',3)
    plot(vertex_coord(1,[1,4]),vertex_coord(2,[1,4]),'b','LineWidth',3)
    plot(vertex_coord(1,[4,3]),vertex_coord(2,[4,3]),'b','LineWidth',3)
    plot(vertex_coord(1,[4,8]),vertex_coord(2,[4,8]),'b','LineWidth',3)
    plot(vertex_coord(1,[3,7]),vertex_coord(2,[3,7]),'b','LineWidth',3)
    plot(vertex_coord(1,[2,6]),vertex_coord(2,[2,6]),'b','LineWidth',3)
    plot(vertex_coord(1,[1,5]),vertex_coord(2,[1,5]),'b','LineWidth',3)
    plot(vertex_coord(1,[8,5]),vertex_coord(2,[8,5]),'b','LineWidth',3)
    plot(vertex_coord(1,[5,6]),vertex_coord(2,[5,6]),'b','LineWidth',3)
    plot(vertex_coord(1,[7,6]),vertex_coord(2,[7,6]),'b','LineWidth',3)
    plot(vertex_coord(1,[7,8]),vertex_coord(2,[7,8]),'b','LineWidth',3)
    
end

