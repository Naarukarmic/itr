
% SLAM2D  A 2D EKF-SLAM algorithm with simulation and graphics.
%
%  HELP NOTES:
%  1. The robot state is defined by [xr;yr;ar] with [xr;yr] the position and
%     [ar] the orientation angle in the plane.
%  2. The landmark states are simply Li=[xi;yi]. They are stored in a internal
%     variable of the simulator.
%  3. The control signal for the robot is u=[dx;da] where [dx] is a forward
%     motion and [da] is the angle of rotation. It is fixed in the simulator
%     and accessible using the function sim_get_control_signal()
%  4. The motion perturbation is additive Gaussian noise n=[nx;na] with
%     covariance Q, which adds to the control signal. It is computed from the
%     standard deviation vector q.
%  5. The measurements are range-and-bearing Yi=[di;ai], with [di] the
%     distance from the robot to landmark Li, and [ai] the bearing angle from
%     the robot's x-axis. The measurements are accessible through the function
%     sim_get_lmk_measurement(i)
%  6. The true map is a unknown vector of the form
%     [xr;yr;ar;x1;y1;x2;y2;x3;y3; ... ;xN;yN]
%  7. The estimated map is Gaussian, defined by
%       x: mean of the map
%       P: covariances matrix of the map
%  8. The estimated entities (robot and landmarks) are extracted from {x,P}
%     via "pointers", which are just indices in the matrices x,P that define
%     the Gaussian. We suggest you use the following nomenclature, denoted in
%     small letters:
%       r:  pointer to robot state. Usually the first three elements:
%           r=[1,2,3]
%       l:  pointer to a landmark. For example, if we have several landmarks
%           in the map, then l=[4,5] is the pointer for the first landmark,
%           l=[6,7] for the second, and so on.
%       m:  pointers to all used landmarks. You should build it based on the
%           current known landmarks. For example, for 3 landmarks you would
%           have m=[4,5,6,7,8,9]. See lm_all_lmk_pointers helper function
%           further below.
%       rl: pointers to robot and one landmark. Build it from r and l, as rl =
%           [r;l]
%       rm: pointers to robot and all landmarks (the currently used map).
%           Build it using r and m vectors with rm=[r;m]
%     Therefore:  x(r)     is the robot state,
%                 x(l)     is the state of one landmark
%                 P(r,r)   is the covariance of the robot
%                 P(l,l)   is the covariance of one landmark
%                 P(r,l)   is the cross-variance between robot and a lmk
%                 P(rm,rm) is the current full covariance -- the rest is
%                 unused.
%     NOTE: Pointers are always row-vectors of integers.
%  9. Managing the map space is done through map management helper functions,
%     named with prefix mm_*. Use it as follows:
%       * mm_query_space(n)  -> returns pointer fs to n free spaces. It will
%                               return a pointer fs to the space available.
%                               length(fs) will be n if there is at least n
%                               spaces available, and less than n if there are
%                               less than n spaces available.
%       * mm_block_space(fs) -> block positions indicated in vector fs
%       * mm_free_space(fs)  -> liberate positions indicated in vector fs
%     NOTE: Don't forget to check if the number of elements in fs matches the
%           number of queried spaces.
% 10. Managing the existing landmarks is done through the use of landmark
%     management helper functions, named with prefix lm_*. Use it as follows:
%       * lm_find_non_mapped_lmk()          -> look for one non-mapped
%                                              landmark
%       * lm_associate_pointer_to_lmk(fs,i) -> associate free space pointer fs
%                                              to landmark i
%       * lm_lmk_pointer(i)                 -> recover a landmark pointer l
%       * lm_all_lmk_pointers()             -> recover pointers to all known
%                                              landmarks
%       * lm_forget_lmk(i)                  -> forget landmark i
%       * lm_all_lmk_ids()                  -> recover the id of all known
%                                              landmarks
% 11. The simulation can be accessed using sim_* functions. The available
%     functions are:
%       * sim_get_control_signal()     -> recover the current control signal
%       * sim_get_lmk_measurement(i)   -> recover measurement to landmark i
%       * sim_get_initial_robot_pose() -> recover the initial pose of the
%                                         robot
%       * sim_simulate_one_step()      -> perform one step of simulation. It
%                                         is already used where it should be
%                                         called. Do NOT use it again.
% 12. The init_* functions  are used to initialize the simulator. You should
%     not use nor change them in the code below.
%
%   (c) 2010, 2011, 2012, 2013, 2014, 2015, 2016 Joan Sola.
%   (c) 2016, 2017 Ellon Paiva Mendes.


% I. INITIALIZE ===========================================================

% I.1 SIMULATOR -----------------------------------------------------------

init_simulator

% I.2 ESTIMATOR -----------------------------------------------------------

% a. Define Map: Gaussian {x,P}
%   mapsize: size of the map
mapsize = 100;
%   x: state vector's mean
x = zeros(mapsize,1);
%   P: state vector's covariances matrix
P = zeros(mapsize,mapsize);

% b. Define system noise: Gaussian {0,Q}
q = [.01; .02];      % amplitude or standard deviation
Q = diag(q.^2); % covariance matrix, built from the standard deviation

% c. Define measurement noise: Gaussian {0,S}
s = [0.1; 1*pi/180];      % amplitude or standard deviation
S = diag(s.^2); % covariance matrix, built from the standard deviation

% d. Map management
init_map_management(mapsize); % See Help Note #12 above.

% e. Landmarks management
init_landmark_management;     % See Help Note #12 above.

% f. Initialize robot in map
% query for map space (see note 9)
r = mm_query_space(3); % get pointers to robot space in the map.
mm_block_space(r);     % block map positions

% init mean and covariance
x(r)   = sim_get_initial_robot_pose;    % initialize robot state. See Help Note #11 above.
P(r,r) = 0;    % initialize robot covariance

% I.3 GRAPHICS ------------------------------------------------------------

init_graphics





% II. TEMPORAL LOOP =======================================================

for t = 1:200
    
    
    % II.1 SIMULATOR ------------------------------------------------------
    
    sim_simulate_one_step();
    
    % II.2 ESTIMATOR ------------------------------------------------------
    
    % NOTE: YOU MUST COMPLETE THIS WHOLE SECTION BY ENTERING YOUR CODE.
    %       After coding the function observe.m, uncoment all the lines
    %       below _once_ and follow the instructions in the remaining
    %       comments.
    %       Tip: Select all the lines in this section and press ctrl+t once
    %       to remove one comment level.
    
    % a. create useful map pointers to be used hereafter
    %-----------------------------------------------------
    % vector with all pointers to known landmarks (see note 10)
    % NOTE: once you defined m pointer, add it to the draw_graphics
    %       function in the end of this file.
    m  = lm_all_lmk_pointers();
    % vector with pointers to all used states: robot and known landmarks
    % (see note 8)
    rm = [r,m]; 
    
    
    % b. Prediction -- robot motion
    %-------------------------------
    
    % Update {x(r), P}: using the function move(),
    %   compute new robot position x(r),
    %   and obtain Jacobians R_r, R_n.
    %   (see note 11)
    [x(r), R_r, R_n] = move(x(r), sim_get_control_signal()); % TODO: noise
    % Then update covariances matrix P --> use sparse formulation, e.g.:
    P(r,m) = R_r * P(r,m);
    P(m,r) = P(r,m)';
    P(r,r) = R_r * P(r,r) * R_r' + R_n * Q * R_n'; % TODO
    
    
    % c. Landmark correction -- known landmarks
    %-------------------------------------------
    
    % c1. obtain all indices of existing landmarks (see note 10)
    lids = lm_all_lmk_ids(); 
    
    % c2. loop for all known landmarks
    for i = lids
        
        % c3. Obtain pointers to landmark 'i' (see note 10)
        l = lm_lmk_pointer(i); % landmark's pointers vector
        
        % c4. compute expectation Gaussian {e,E}  --- this is h(x) in your
        % notes
        [e, E_r, E_l] = observe(x(r), x(l));
        E_rl = [E_r, E_l];
        % P_rl_rl = [P(r,r), P(r,l); P(l,r), P(l,l)];
        % E = E_rl * P_rl_rl * E_rl';
        rl = [r, l];
        E = E_rl * P(rl, rl) * E_rl';
        
        % c5. get measurement Yi for landmark 'i', as Yi=[dist,angle]' (see
        % note 11)
        Yi = sim_get_lmk_measurement(i);
        
        % c6. compute innovation --- this is y-h(x), or y-e as we have
        % e=h(x)
        z = Yi - e;
        % remember to bring angular innovations z(2) around zero, not
        % multiples of 2pi !
        if z(2) > pi
            z(2) = z(2) - 2*pi;
        else if z(2) < -pi
            z(2) = z(2) + 2*pi;
            end
        end
        
        % c7. compute innovation covariance
        Z = E + S;
        
        % Mahalanobis test (Ask BE responsible for justification)
        if z' * Z^-1 * z < 9
            
            % c8. compute Kalman gain
            % P_rml = [P(r,r), P(r,l); P(m,r), P(m,l)];
            % K = P_rml * E_rl' * Z^-1;
            K = P(rm, rl) * E_rl' * Z^-1;
            
            % c9. Perform Kalman update
            x(rm)    = x(rm) + K * z;
            P(rm,rm) = P(rm,rm) - K * Z * K';
            
        end % Mahalanobis test
        
    end % loop for all known landmarks
    
    
    
    % d. Landmark Initialization -- one new landmark only at each iteration
    %----------------------------------------------------------------------
    
    % d1. Get ID of a non-mapped landmark (see note 10)
    i = lm_find_non_mapped_lmk();
    if ~isempty(i)
        % d2. Get pointers vector of the new landmark in the map (see
        % note 9)
        l = mm_query_space(2);  
        
        % if there is still space for a new landmark in the map
        if numel(l) >= 2
            
            % d3. block map space (see note 9)
            mm_block_space(l);     
            
            % d4. store landmark pointers (see note 10)
            lm_associate_pointer_to_lmk(l, i);    
            
            % d5. measurement Yi of landmark 'i'  (see note 11)
            Yi = sim_get_lmk_measurement(i);
            
            % d6. Landmark initialization in the map. Update x and P:
            %   get x(l), landmark state
            %   get Jacobians L_r and L_y
            [x(l), L_r, L_y] = invObserve(x(r), Yi)
            % P(l,rm)          = L_r * [P(r,r), P(r,m)];
            P(l,rm)          = L_r * P(r,rm);
            P(rm,l)          = P(l,rm)';
            P(l,l)           = L_r * P(r,r) * L_r' + L_y * S * L_y';
            
        end % if numel(l) >= 2
    end % if ~isempty(i)
    
    % II.3 GRAPHICS -------------------------------------------------------
    
    % a. Add the pointer m as the last argument to the draw_graphics
    draw_graphics(x,P,r,m)
    
    % If needed, use next line to slow down the loop
    % pause(1)
end


