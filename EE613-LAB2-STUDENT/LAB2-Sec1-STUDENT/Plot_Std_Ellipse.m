function h=Plot_Std_Ellipse(M,V,EllipseColor)
[Ev,D] = eig(V);
d = length(M);
if V(:,:)==zeros(d,d),
    V(:,:) = ones(d,d)*eps;
end
iV = inv(V);
% Find the larger projection
P = [1,0;0,0];  % X-axis projection operator
P1 = P * 2*sqrt(D(1,1)) * Ev(:,1);
P2 = P * 2*sqrt(D(2,2)) * Ev(:,2);
if abs(P1(1)) >= abs(P2(1)),
    Plen = P1(1);
else
    Plen = P2(1);
end
count = 1;
step = 0.001*Plen;
Contour1 = zeros(2001,2);
Contour2 = zeros(2001,2);
for x = -Plen:step:Plen,
    a = iV(2,2);
    b = x * (iV(1,2)+iV(2,1));
    c = (x^2) * iV(1,1) - 1;
    Root1 = (-b + sqrt(b^2 - 4*a*c))/(2*a);
    Root2 = (-b - sqrt(b^2 - 4*a*c))/(2*a);
    if isreal(Root1),
        Contour1(count,:) = [x,Root1] + M';
        Contour2(count,:) = [x,Root2] + M';
        count = count + 1;
    end
end
Contour1 = Contour1(1:count-1,:);
Contour2 = [Contour1(1,:);Contour2(1:count-1,:);Contour1(count-1,:)];
axis([0 3 0 3]);
plot(M(1),M(2),'k+');axis([0 3 0 3]);
h=plot(Contour1(:,1),Contour1(:,2),EllipseColor);axis([0 3 0 3]);
plot(Contour2(:,1),Contour2(:,2),EllipseColor);axis([0 3 0 3]);
%[LEGH,OBJH,OUTH,OUTM] = legend;
%if length(OUTM)==0
%    legend(h,'filsh legend text')
%else
%    legend([OUTH;h],OUTM{:},'New legend text')
%end
   
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%% End of Plot_Std_Ellipse %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%